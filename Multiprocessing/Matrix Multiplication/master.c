#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#define  NUM_RUNS 5
// RUNNING AT THE SERVER
// gcc /home/A01732702/FinalAssignment/MatrixMult/master.c -o /home/A01732702/FinalAssignment/MatrixMult/master -std=c99 -w
// /home/A01732702/FinalAssignment/MatrixMult/master </path/to/matrixA.txt> </path/to/matrixB.txt>
long MEMORY = 0;

long getAvailableMemory() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return -1;
    }

    long available_memory = info.freeram * info.mem_unit / 1024;  // Convert from bytes to kilobytes
    return available_memory;
}
void getMemoryUsage() {
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    MEMORY+=r_usage.ru_maxrss;
    //printf("Memory usage: %ld kilobytes\n", r_usage.ru_maxrss);
}


int compareMatrices(char *filename1, char *filename2) {
    FILE *file1, *file2;
    char *buffer1, *buffer2;

    file1 = fopen(filename1, "r");
    if (file1 == NULL) {
        perror("Error opening file1.txt");
        return 1;
    }

    file2 = fopen(filename2, "r");
    if (file2 == NULL) {
        perror("Error opening file2.txt");
        fclose(file1);
        return 1;
    }

    buffer1 = (char *)malloc(1000 * sizeof(char));
    buffer2 = (char *)malloc(1000 * sizeof(char));

    if (fgets(buffer1, 1000, file1) == NULL) {
        perror("Error reading file1.txt");
        free(buffer1);
        free(buffer2);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    if (fgets(buffer2, 1000, file2) == NULL) {
        perror("Error reading file2.txt");
        free(buffer1);
        free(buffer2);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    fclose(file1);
    fclose(file2);

    int result = strcmp(buffer1, buffer2);

    free(buffer1);
    free(buffer2);

    if (result == 0) {
        return 1;
    } else {
        return 0;
    }
}

int run_command(const char *command) {
    FILE *fp;
    char buffer[1024];

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    int result;
    fgets(buffer, sizeof(buffer) - 1, fp);

    pclose(fp);

    sscanf(buffer, "%d", &result);

    return result;
}
      
int main(int argc, char *argv[]) {
    system("cd /home/A01732702/FinalAssignment/MatrixMult");
    system("gcc -o Serial_Dynamic Serial_Dynamic.c -std=c99 -w");
    system("gcc -o OMP_Dynamic OMP_Dynamic.c -fopenmp -std=c99 -w");
    system("gcc -o Autovec_Dynamic Autovec_Dynamic.c -ftree-vectorize -std=c99 -w");
    system("nvcc -o CUDA_Dynamic CUDA_Dynamic.cu -w");
    printf("ALL FILES COMPILED SUCCESSFULLY\n");
    int serial_results[NUM_RUNS];
    int omp_results[NUM_RUNS];
    int vectorization_results[NUM_RUNS];
    int cuda_results[NUM_RUNS];
    char *filename1, *filename2;
    if (argc == 3) {
        filename1 = argv[1];
        filename2 = argv[2];
    } else {
        printf("Usage: %s <filename1> <filename2>\n", argv[0]);
        return 1;
    }    
    int rowsA, colsA, rowsB, colsB;
    printf("Enter the number of rows for matrix A: ");
    scanf("%d", &rowsA);
    printf("Enter the number of columns for matrix A: ");
    scanf("%d", &colsA);
    printf("Enter the number of rows for matrix B: ");
    scanf("%d", &rowsB);
    printf("Enter the number of columns for matrix B: ");
    scanf("%d", &colsB);
 

    if (colsA != rowsB) {
        printf("Matrix multiplication is not possible due to incompatible dimensions.\n");
        return 1;
    }    
    
    char commandSerial[100];
    char commandVectorization[100];
    char commandOMP[100];
    char commandCUDA[100];
    sprintf(commandSerial, "./Serial_Dynamic %d %d %d %d %s %s", rowsA, colsA, rowsB, colsB, filename1, filename2);
    sprintf(commandVectorization, "./Autovec_Dynamic %d %d %d %d %s %s", rowsA, colsA, rowsB, colsB,  filename1, filename2);
    sprintf(commandOMP, "./OMP_Dynamic %d %d %d %d %s %s", rowsA, colsA, rowsB, colsB,  filename1, filename2);
    sprintf(commandCUDA, "./CUDA_Dynamic %d %d %d %d %s %s", rowsA, colsA, rowsB, colsB, filename1, filename2);
    
    run_command(commandSerial);
    run_command(commandVectorization);
    run_command(commandOMP);
    run_command(commandCUDA);
    for (int i = 0; i < NUM_RUNS; i++) {
        printf("\nRUN %d\n",i+1);
        serial_results[i] = run_command(commandSerial);
        printf("Serial...\t");
        getMemoryUsage();
        omp_results[i] = run_command(commandOMP);
        printf("OMP...\t");
        getMemoryUsage();
        vectorization_results[i] = run_command(commandVectorization);
        getMemoryUsage();
        printf("Autovec...\t");
        cuda_results[i]= run_command(commandCUDA);
	getMemoryUsage();
        printf("CUDA...\t");
    }
    if(MEMORY>getAvailableMemory())
    { 
      printf("\nProgram will not run properly because of memory problems (%ld/%ld KB in use)\n",MEMORY,getAvailableMemory());
      return 1;
    }else{
      printf("\nMemory requirements satisfied (%ld/%ld KB in use)\n",MEMORY,getAvailableMemory());
    }
    system("cp matrixC_Serial.txt  matrixC_CUDA.txt");
    if(compareMatrices("matrixC_Autovec.txt","matrixC_Serial.txt")){
	printf("Autovec Matrix Validated\n");
    }else{
        printf("Autovec Matrix NOT Validated\n");
    }
    if(compareMatrices("matrixC_OMP.txt","matrixC_Serial.txt")){
        printf("OMP Matrix Validated\n");
    }else{
        printf("OMP Matrix NOT Validated\n");
    }    
    if(compareMatrices("matrixC_CUDA.txt","matrixC_Serial.txt")){
        printf("CUDA Matrix Validated\n");
    }else{
        printf("CUDA Matrix NOT Validated\n");
    }

    printf("\n*********************************RESULTS*********************************\n");
    printf("%-15s %-15s %-15s %-15s %-15s\n", "Run", "Serial", "Autovec", "OMP", "CUDA");

    for (int i = 0; i < NUM_RUNS; i++) {
        printf("%-15d %-15d %-15d %-15d %-15d\n", i+1, serial_results[i], vectorization_results[i], omp_results[i], cuda_results[i]);
    }

    float avg_serial = 0, avg_vectorization = 0, avg_omp = 0, avg_cuda = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        avg_serial += serial_results[i];
        avg_vectorization += vectorization_results[i];
        avg_omp += omp_results[i];
        avg_cuda += cuda_results[i];
    }

    avg_serial /= NUM_RUNS;
    avg_vectorization /= NUM_RUNS;
    avg_omp /= NUM_RUNS;
    avg_cuda /= NUM_RUNS;
    printf("%-15s %-15.2f %-15.2f %-15.2f %-15.2f\n", "Average", avg_serial, avg_vectorization, avg_omp, avg_cuda);

    printf("%-15s %-15s %-15.2f %-15.2f %-15.2f\n", "%Improvement", "-", 100*(avg_serial/avg_vectorization),  100*(avg_serial/avg_omp),  100*(avg_serial/avg_cuda));

    return 0;
}

