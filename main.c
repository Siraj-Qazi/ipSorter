
//Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structure definition
typedef struct ip_address_t {
    int oct1;
    int oct2;
    int oct3;
    int oct4;
    char alias[100];
}
        address_t;

//Global Variables Definitions
address_t *all_addresses;
int **locality_t;
char user_name[100];
int ip_index = 0;
int *printed_oct1, *printed_oct2;

// Function Prototypes
address_t *getAddress(char *);
void print_addresses(address_t *, int);
void Generate_Locality_Rpt(void);
void Generate_Alias_List(void);
void InsertionSort(address_t*);
char *getMonth(int);
int printed(address_t);
void Read_Data_File(char*);

// main function
int main() {
    printf("\n Enter user name: ");
    scanf("%[^\n]s", user_name);

    //print_addresses(all_addresses, ip_index);
    Read_Data_File("CS222_Inet.txt");       // Read file
    Generate_Locality_Rpt();                         // Generate locality report
    Generate_Alias_List();                           // Generate alias listing
    return 0;
}

// Read in network address from string into a structure
address_t *getAddress(char *input_file_line) {
    address_t *addr = (address_t *) malloc(sizeof(address_t));
    char *temporary_var = strdup(input_file_line);
    char *token_str = strtok(temporary_var, " ");
    sscanf(token_str, "%d.%d.%d.%d", &addr->oct1, &addr->oct2, &addr->oct3, &addr->oct4);
    temporary_var = strtok(NULL, " ");
    sscanf(temporary_var, "%s", addr->alias);
    return addr;
}

//function to print all addresses : DEBUG
void print_addresses(address_t *addressList, int count) {
	int i;
    for (i = 0; i < count; ++i)
        printf("\n %d.%d.%d.%d %s",
               addressList[i].oct1,
               addressList[i].oct2,
               addressList[i].oct3,
               addressList[i].oct4,
               addressList[i].alias);

    printf("\n");
}

// Reading data from file and storing into structure array
void Read_Data_File(char* file_name) {
    FILE *fp = fopen(file_name, "r");

    // Check if file exists
    if (fp == NULL) {
        printf("Could not open file %s", file_name);
        return;
    };

    char c;
    int count = 1;
    // Count number of ip addresses
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;

    //printf("\n No. of IP Addresses = %d\n",count);
    fclose(fp);
    fp = fopen(file_name, "r");

    all_addresses = (address_t *) malloc(count * sizeof(address_t));
    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        all_addresses[ip_index] = *getAddress(line);
        if (strcmp(all_addresses[ip_index].alias, "none") == 0) {
            break;
        }
        ip_index++;
    }

    locality_t = (int **) malloc(ip_index * sizeof(int *));
    int i;
    for (i = 0; i < ip_index; ++i) {
        locality_t[i] = (int *) malloc(2 * sizeof(int));
        locality_t[i][0] = all_addresses[i].oct1;
        locality_t[i][1] = all_addresses[i].oct2;
    }
}

//Generate network localtity report
void Generate_Locality_Rpt() {

    FILE *fp = fopen("222 Locality Report.txt", "w+");
    // Check if file exists
    if (fp == NULL) {
        printf("Could not write to file %s", "222 Locality Report");
        return;
    };

    printed_oct1 = (int *) malloc(ip_index * sizeof(int));
    printed_oct2 = (int *) malloc(ip_index * sizeof(int));
    int i;
    for (i = 0; i < ip_index; ++i) {
        printed_oct1[i] = 0;
        printed_oct2[i] = 0;
    }


    fprintf(fp, "%s", user_name);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, " %s %d, %d", getMonth(tm.tm_mon + 1),tm.tm_mday, tm.tm_year + 1900);
    fprintf(fp, "\nCS222 Network Locality Report\n");
  
    for (i = 0; i < ip_index; ++i) {

        //printf("\n Processing addr: %d.%d.%d.%d alias: %s\n",all_addresses[i].oct1,all_addresses[i].oct2,all_addresses[i].oct3,all_addresses[i].oct4,all_addresses[i].alias);

        if (!printed(all_addresses[i])) {
            fprintf(fp, "\n%d.%d",
                    all_addresses[i].oct1,
                    all_addresses[i].oct2);
            int j;
            for (j = 0; j < ip_index; ++j) {
                if ((locality_t[i][0] == locality_t[j][0]) && (locality_t[i][1] == locality_t[j][1])) {
                    fprintf(fp, "\n%s", all_addresses[j].alias);
                }
            }

            printed_oct1[i] = all_addresses[i].oct1;
            printed_oct2[i] = all_addresses[i].oct2;
            fprintf(fp, "\n");
        }

    }
    fclose(fp);
}

// Generate Network Alias Listing
void Generate_Alias_List() {
    InsertionSort(all_addresses);
    FILE *fp = fopen("222 Alias List.txt", "w+");
    // Check if file exists
    if (fp == NULL) {
        printf("Could not write to file %s", "222 Locality Report");
        return;
    };

    fprintf(fp, "%s", user_name);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, " %s %d, %d", getMonth(tm.tm_mon + 1),tm.tm_mday, tm.tm_year + 1900);
    fprintf(fp, "\nCS222 Network Alias Listing\n");
    int i;
    for (i = 0; i < ip_index; ++i) {
        fprintf(fp,"\n%s\t%d.%d.%d.%d",all_addresses[i].alias,all_addresses[i].oct1,all_addresses[i].oct2,all_addresses[i].oct3,all_addresses[i].oct4);
    }

    fclose(fp);

}

//Sort structure using insertion sort
void InsertionSort(address_t* addresses)
{
    int j;
    address_t temp;
    int i;
    for(i = 1; i < ip_index; i++)
    {
        j = i - 1;
        while( j >= 0 && strcmp( addresses[j+1].alias, addresses[j].alias ) < 0 )
        {
            temp =  addresses[j + 1];
            addresses[j+1] = addresses[j];
            addresses[j] = temp;
            j--;
        }
    }
}

// Return month corresponding to month number
char *getMonth(int month_no) {
    switch (month_no) {
        case 1:
            return "January";

        case 2:
            return "February";

        case 3:
            return "March";

        case 4:
            return "April";

        case 5:
            return "May";

        case 6:
            return "June";

        case 7:
            return "July";

        case 8:
            return "August";

        case 9:
            return "September";

        case 10:
            return "October";

        case 11:
            return "November";

        case 12:
            return "December";

        default:
            return 0;

    }
}

// Check if a locality has already been printed
int printed(address_t addr) {
	int i;
    for (i = 0; i < ip_index; ++i) {
        if (addr.oct1 == printed_oct1[i] && addr.oct2 == printed_oct2[i]) {
            return 1;
        }
    }
    return 0;
}