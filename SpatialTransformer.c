#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <unistd.h>

/** Written by Abraham Kabon **/
/* This program performs a Galilean transformation on a set of points in n-dimensions by
computing the minimum and maximum values of each coordinate and centering each point around
the midpoint of their range. It also calculates the largest scale difference among dimensions.
You can save this data to a .txt file.
*/

/*Useful for rendering, object manipulation, or aligning objects in a coordinate system for
better visualization of plots and graphics.
*/

/*Future additions includes but are not limited to exploring other linear/ geometric transformations useful for
solving engineering problems and visualizing conceptual physics.
*/

// CONSTANTS //
# define file_checker "filename.txt"
# define exit_file "exit"
# define PI 3.141592653589793

// FUNCTION PROTOTYPES //
double valid_coordinate_input(int i, int j, char **dimensionlabel);
void find_min_max(double **points, int size, int dimensions, int n, double **minmax_values);
void center_points(double **points, int size, int dimensions, int n, double **minmax_values);
void transform_points(double **points, int size, int dimensions, int number_of_points, double *frame_of_reference);

int main() {
    // VARIABLES //
    int i, j;
    int n, d;
    double scale = 0;
    char choice1 = 'Y', choice2 = 'Y';
    char c_sys;

    // ARRAYS //
    char filename[100], filename_xext[100];
    char mod_filename[100];

    // FILE POINTER //
    FILE*file;
    FILE*checker;
    FILE*exit;

    // POINTERS //
    char **dlabel = NULL;
    double ** p = NULL;
    double ** minmax = NULL;
    double * del;
    double *ref_frame;

    while(choice1 == 'Y' || choice1 == 'y'){

        printf("SPATIAL-TRANSFORMER.EXE\n");
        printf("==========================================\n");
        printf("Perform Transformation on points in space!\n");
        printf("------------------------------------------\n");
        printf("Press [ENTER] to continue\n");
        getchar();

        system("cls");

        printf("Please Choose a Coordinate System\n");
        printf("=========================================\n");
        printf("[INPUT 1] |  Cartesian  Coordinate System\n");
        printf("[INPUT 2] |    Polar    Coordinate System\n");
        printf("[INPUT 3] | Cylindrical Coordinate System\n");
        printf("[INPUT 4] |  Spherical  Coordinate System\n");
        printf("-----------------------------------------\n");
        while(1){
            scanf(" %c", &c_sys);
            printf("-----------------------------------------\n");
            if(c_sys == '1'){
                printf("Input Number of Dimensions (d): ");
                scanf("%d", &d);
                printf("-----------------------------------------\n");
                break;
            }
            else if(c_sys == '2'){
                d = 2;
                break;
            }
            else if(c_sys == '3' || c_sys == '4'){
                d = 3;
                break;
            } else{
                printf("Error: Invalid Input!\n");
                printf("-----------------------------------------\n");
                }
            }

        printf("Input Number of Points (n): ");
        scanf("%d", &n);

        p = (double**)malloc(n * sizeof(double*));
        for (i = 0; i<n; i++) {
            p[i] = (double*)malloc(d * sizeof(double));
            } /*Dynamic memory allocation for array of points to be stored in memory*/

        minmax = (double**)malloc(2 * sizeof(double*));
        for (i=0; i<2; i++) {
            minmax[i] = (double*)malloc(d * sizeof(double));
        } /*Memory allocation for array of maximum and minimum values*/

        dlabel = (char**)malloc(d * sizeof(char*));
        for (i=0; i<d; i++){
            dlabel[i] = (char*)malloc(100*sizeof(char));
        } /*Labels*/

        del = (double*)malloc(d * sizeof(double)); /*to store xmax - xmin in memory*/

        switch (c_sys){
            case '2':
                strcpy(dlabel[0], "r");
                strcpy(dlabel[1], "theta");
                break;
            case '3':
                strcpy(dlabel[0], "rho");
                strcpy(dlabel[1], "phi");
                strcpy(dlabel[2], "z");
                break;
            case '4':
                strcpy(dlabel[0], "r");
                strcpy(dlabel[1], "theta");
                strcpy(dlabel[2], "phi");
                break;
        }

        if (d <= 3 && c_sys == '1') {
            switch (d) {
                case 1:
                    printf("\nThis is a %d-dimensional problem. Your coordinate will be labeled as 'x'.\n", d);
                    strcpy(dlabel[0], "x");
                    break;
                case 2:
                    printf("\nThis is a %d-dimensional problem. Each coordinate will be labeled in 'x' and 'y'.\n", d);
                    strcpy(dlabel[0], "x"); strcpy(dlabel[1], "y");
                    break;
                case 3:
                    printf("\nThis is a %d-dimensional problem. Each coordinate will be labeled in 'x','y', and 'z'.\n", d);
                    strcpy(dlabel[0], "x"); strcpy(dlabel[1], "y"); strcpy(dlabel[2], "z");
                    break;
                }
            }

        if (d > 3 && c_sys == '1'){
            printf("\nThis appears to be a higher dimensional problem. Please label each dimension!");
            printf("\n-----------------------------------------------------------------------------\n");
            for(i=0; i<d; i++){
                printf("d%d: ", i+1);
                scanf("%s", dlabel[i]);
                printf("\n");
            }
        }

        getchar();
        printf("-------------------------\n");
        printf("Press [ENTER] to proceed:");
        getchar();

        for (i = 0; i<n; i++) {
            for (j = 0; j<d; j++) {
                system("cls");
                printf("Enter value for the %s-coordinate for p%d: ", dlabel[j], i+1);
                if(c_sys == '2' || c_sys == '3' || c_sys == '4'){
                    printf("\n-----------------------------------------");
                    printf("\n(Enter all angles in degrees)");
                }
                p[i][j] = valid_coordinate_input(i, j, dlabel);
            }
        }

        if (c_sys == '2'||c_sys == '3'){
            for (i=0; i<n; i++){
                    p[i][0] = p[i][0]*cos(p[i][1]*(PI/180));
                    p[i][1] = p[i][0]*sin(p[i][1]*(PI/180));
            }
        } else if(c_sys == '4'){
            for(i=0; i<n; i++){
                p[i][0] = p[i][0]*sin(p[i][2]*(PI/180))*cos(p[i][1]*(PI/180));
                p[i][1] = p[i][0]*sin(p[i][2]*(PI/180))*sin(p[i][1]*(PI/180));
                p[i][2] = p[i][0]*cos(p[i][2]*(PI/180));
            }
        }

        system("cls");
        printf("[INPUT 1] | Transform these points to your specified reference frame?\n");
        printf("[INPUT 2] | Center each point about the midpoint of their range\n");
        printf("---------------------------------------------------------------------\n");
        printf("Pick or choice to proceed: ");
        scanf(" %c", &choice2);

        switch(choice2){
            case '1':
                ref_frame = (double*)malloc(d*sizeof(double));
                for(i=0; i<d; i++){
                    system("cls");
                    printf("Enter the %s-coordinate of your reference frame: ", dlabel[i]);
                    while(scanf("%lf", &ref_frame[i])!= 1){
                        printf("\nError: Invalid input!");
                        printf("\n---------------------");
                        printf("\nEnter the %s-coordinate of your reference frame: ", dlabel[j]);
                    }
                }
                if (c_sys == '2'||c_sys == '3'){
                    ref_frame[0] = ref_frame[0]*cos(ref_frame[1]*(PI/180));
                    ref_frame[1] = ref_frame[0]*sin(ref_frame[1]*(PI/180));
                } else if(c_sys == '4'){
                    ref_frame[0] = ref_frame[0]*sin(ref_frame[2]*(PI/180))*cos(ref_frame[1]*(PI/180));
                    ref_frame[1] = ref_frame[0]*sin(ref_frame[2]*(PI/180))*sin(ref_frame[1]*(PI/180));
                    ref_frame[2] = ref_frame[0]*cos(ref_frame[2]*(PI/180));
                }

                transform_points(p, n, d, n, ref_frame);
                find_min_max(p, n, d, n, minmax);
                free(ref_frame);
                break;
            case '2':
                // FUNCTION CALL //
                center_points(p, n, d, n, minmax);
                break;
        }

        for (i=0; i<d; i++){
            del[i] = minmax[1][i] - minmax[0][i];
        }

        for (i = 0; i<d; i++) {
            if (scale < del[i]) {
                scale = del[i];
            }
        }

        if(c_sys == '2' || c_sys == '3'){
            printf("Displaying results...\n");
            printf("--------------------------------------------------------\n");
            printf("[INPUT 1] to display in Polar or Cylindrical Coordinates\n");
            printf("[INPUT 2] to display in Cartesian Coordinates\n");
            printf("--------------------------------------------------------\n");
            while(1){
                scanf(" %c", &choice2);
                if(choice2 == '1'){
                    for (i=0; i<n; i++){
                        double x = p[i][0];
                        double y = p[i][1];
                        p[i][0] = sqrt(x * x + y * y);
                        p[i][1] = atan2(y, x) * (180 / PI);
                    }
                    break;
                } else if (choice2 == '2'){
                    switch(d){
                        case 2:
                            strcpy(dlabel[0], "x"); strcpy(dlabel[1], "y");
                            break;
                        case 3:
                            strcpy(dlabel[0], "x"); strcpy(dlabel[1], "y"); strcpy(dlabel[2], "z");
                            break;
                    }
                    break;
                } else{
                    printf("Error: Invalid Input!");
                    printf("--------------------------------------------------------\n");
                }
            }
        } else if(c_sys == '4'){
              printf("Displaying results...\n");
              printf("---------------------------------------------\n");
              printf("[INPUT 1] to display in Spherical Coordinates\n");
              printf("[INPUT 2] to display in Cartesian Coordinates\n");
              while(1){
                  scanf("%c", &choice2);
                  if(choice2 == '1'){
                      for (i=0; i<n; i++){
                          double x = p[i][0];
                          double y = p[i][1];
                          double z = p[i][2];

                          // To convert back to spherical coordinates //
                          double r = sqrt(x * x + y * y + z * z);
                          double theta = acos(z / r) * (180 / PI);
                          double phi = atan2(y, x) * (180 / PI);

                          // Store the spherical coordinates back into the array
                          p[i][0] = r;
                          p[i][1] = theta;
                          p[i][2] = phi;
                      }
                      break;
                  } else if (choice2 == '2'){
                      strcpy(dlabel[0], "x"); strcpy(dlabel[1], "y"); strcpy(dlabel[2], "z");
                      break;
                  } else{
                      printf("Error: Invalid Input!");
                      printf("---------------------------------------------\n");
                  }
              }
          }

        // Print results //
        system("cls");
        printf("\nRESULTS\n");
        printf("========================================\n");
        switch(choice2){
            case '1':
                printf("Transformed Points:\n");
                break;
            case '2':
                printf("Centered Points:\n");
                break;
        }

        printf("========================================\n");
        printf("Point\t");
        for (j=0; j<d; j++) {
            printf("%s\t\t", dlabel[j]);
            }
        printf("\n----------------------------------------\n");
        for (i = 0; i<n; i++) {
            printf("P%d\t", i + 1);
            for (j = 0; j<d; j++) {
                printf("%lf\t\t", p[i][j]);
            }
            printf("\n");
        }
        printf("----------------------------------------\n");
        printf("Largest Scale Difference: %lf\n", scale);
        printf("Number of Points (n): %d\n", n);
        printf("========================================\n");

        free(minmax);
        free(del);

        while(1){
            printf("Save these results to a file?(Y/N): ");
            scanf(" %c", &choice2);
            if(choice2 == 'Y' || choice2 == 'y'){
                system("cls");
                printf("\nEnter the file name: ");
                getchar();
                fgets(filename_xext, sizeof(filename_xext), stdin);

                size_t len_xext = strlen(filename_xext);
                if (len_xext > 0 && filename_xext[len_xext - 1] == '\n'){
                    filename_xext[len_xext - 1] = '\0';
                }
                snprintf(filename, sizeof(filename), "%s.txt", filename_xext);

                size_t len = strlen(filename);
                if (len > 0 && filename_xext[len - 1] == '\n'){
                    filename[len - 1] = '\0';
                }

                file = fopen(filename, "r");

                if (file == NULL){
                    file = fopen(filename, "w");
                    if (file == NULL){
                        printf("Error: Could not open file");
                        return 1;
                    }
                }
                else{
                    fclose(file);
                    printf("\n===========================\n");
                    printf("This file already exists!\n");
                    printf("----------------------------------------------------\n");

                    while(1){
                        printf("Do you wish to overwrite the existing file? [Y/N]: ");
                        scanf(" %c", &choice1);

                        switch (choice1) {
                            case 'Y':
                            case 'y':
                                file = fopen(filename, "w");
                                break;
                            case 'N':
                            case 'n':
                                i = 1;
                                while (i < i+1){
                                    sprintf(mod_filename, "%s(%d).txt", filename_xext, i);
                                    file = fopen(mod_filename, "r");
                                    if (file == NULL){
                                        file = fopen(mod_filename, "w");
                                        break;
                                    }
                                    else {
                                        i++;
                                    }
                                }
                            }
                        if(choice1 == 'Y' || choice1 == 'y' || choice1 == 'N' || choice1 =='n'){
                            break;
                            } else{
                                printf("Error: Invalid Input!\n");
                                printf("---------------------\n");
                                sleep(2);
                            }
                        }
                    }
            for (i=0; i<n; i++){
                for (j=0; j<d; j++){
                fprintf(file, "%lf         ", p[i][j]);
                    }
                fprintf(file, "\n");
                }
            fprintf(file, "%lf\n", scale);
            fprintf(file, "%d", n);

            free(p);
            free(dlabel);

            switch (choice1){
                case 'Y':
                case 'y':
                    printf("====================================================\n");
                    printf("File Saved! This is your filename: %s\n", filename);
                    checker = fopen(file_checker, "w");
                    fprintf(checker, "%s", filename);
                    fclose(checker);
                    break;
                case 'N':
                case 'n':
                    printf("====================================================\n");
                    printf("File Saved! This is your filename: %s\n", mod_filename);
                    checker = fopen(file_checker, "w");
                    fprintf(checker, "%s", mod_filename);
                    fclose(checker);
                    break;
                default:
                    printf("Program Error\n");
            }
            sleep(2);
            fclose(file);
            break;
            } else if (choice2 == 'N' || choice2 == 'n'){
                break;
            } else {
                printf("Error: Invalid Input!\n");
                printf("---------------------\n");
                sleep(2);
            }
        }
    printf("================================================\n");
    while(1){
        printf("Do you wish to input new sets of points?[Y/N]: ");
        scanf(" %c", &choice1);

        if(!(choice1 == 'y' || choice1 == 'Y' || choice1 == 'n' || choice1 == 'N')){
            printf("Error: Invalid Input!\n");
            printf("---------------------\n");
            sleep(2);
            }
        break;
        }
    }
    exit = fopen(exit_file, "w");
    fclose(exit);
    sleep(3);
    return 0;
}

// FUNCTION DEFINITIONS //
double valid_coordinate_input(int i, int j, char **dimensionlabel){
    double coordinate;
    while (scanf("%lf", &coordinate) != 1){
        printf("Invalid Input! Please Enter a Valid Number: ");
        while (getchar() != '\n');
    }
    return coordinate;
}

void find_min_max(double **points, int size, int dimensions, int number_of_points, double **minmax_values) {
    for (int j = 0; j < dimensions; j++) {
        minmax_values[0][j] = DBL_MAX;
        minmax_values[1][j] = -DBL_MAX;
    }
    for (int i=0; i<number_of_points; i++) {
        for (int j=0; j<dimensions; j++) {
            if (points[i][j] < minmax_values[0][j]) {
                minmax_values[0][j] = points[i][j];
            }
            if (points[i][j] > minmax_values[1][j]) {
                minmax_values[1][j] = points[i][j];
            }
        }
    }
}

void center_points(double **points, int size, int dimensions, int number_of_points, double **minmax_values) {
    // COMPOSITE FUNCTION CALL //
    find_min_max(points, size, dimensions, number_of_points, minmax_values);
    for (int i=0; i<number_of_points; i++) {
        for (int j=0; j<dimensions; j++) {
            points[i][j] = points[i][j] - ((minmax_values[0][j] + minmax_values[1][j]) / 2);
        }
    }
}

void transform_points(double **points, int size, int dimensions, int number_of_points, double *frame_of_reference){
    for (int i=0; i<number_of_points; i++){
        for (int j=0; j<dimensions; j++){
            points [i][j] = points[i][j] - frame_of_reference[j];
        }
    }
}

