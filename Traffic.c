#include <stdio.h>
#define MAX 10
 
int main()
{
    int n, m;
    int allocation[MAX][MAX], max[MAX][MAX], need[MAX][MAX];
    int available[MAX];
    int finish[MAX] = {0};
    int safeSequence[MAX];
    int trafficDensity[MAX];
    char *direction[] = {"North", "South", "East", "West"};
    int i, j, k, index = 0;
 
    printf("\nSMART TRAFFIC MANAGEMENT SYSTEM\n");
    printf("--------------------------------\n");
    printf("Enter number of traffic directions (max 4): ");
    scanf("%d", &n);
    printf("Enter number of road resources: ");
    scanf("%d", &m);
 
    printf("\nEnter Traffic Density (Number of Vehicles in Each Direction)\n");
    for(i = 0; i < n; i++)
    {
        printf("%s: ", direction[i]);
        scanf("%d", &trafficDensity[i]);
    }
 
    printf("\nEnter Allocation Matrix:\n");
    for(i = 0; i < n; i++)
    {
        printf("%s:\n", direction[i]);
        for(j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);
    }
 
    printf("\nEnter Max Matrix:\n");
    for(i = 0; i < n; i++)
    {
        printf("%s:\n", direction[i]);
        for(j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    }
 
    printf("\nEnter Available Resources:\n");
    for(i = 0; i < m; i++)
        scanf("%d", &available[i]);
 
    int emergency;
    printf("\nEmergency vehicle present? (1-Yes 0-No): ");
    scanf("%d", &emergency);
 
    int emergencyLane;
    if(emergency == 1)
    {
        printf("Enter Emergency Direction (0-North 1-South 2-East 3-West): ");
        scanf("%d", &emergencyLane);
    }
 
    /* Calculate Need Matrix */
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
            need[i][j] = max[i][j] - allocation[i][j];
 
    printf("\nNeed Matrix:\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
 
    /* Emergency vehicle gets priority first */
    if(emergency == 1)
    {
        printf("\nEmergency Vehicle Detected in %s\n", direction[emergencyLane]);
        printf("Giving Immediate Priority...\n");
        safeSequence[index++] = emergencyLane;
        for(j = 0; j < m; j++)
            available[j] += allocation[emergencyLane][j];
        finish[emergencyLane] = 1;
    }
 
    /* Banker's Algorithm */
    for(k = 0; k < n; k++)
    {
        for(i = 0; i < n; i++)
        {
            if(finish[i] == 0)
            {
                int flag = 0;
                for(j = 0; j < m; j++)
                {
                    if(need[i][j] > available[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0)
                {
                    safeSequence[index++] = i;
                    for(j = 0; j < m; j++)
                        available[j] += allocation[i][j];
                    finish[i] = 1;
                }
            }
        }
    }
 
    /* Check safe or deadlock */
    int safe = 1;
    for(i = 0; i < n; i++)
    {
        if(finish[i] == 0)
        {
            safe = 0;
            break;
        }
    }
 
    if(safe == 1)
    {
        printf("\nSafe Traffic Sequence:\n");
        for(i = 0; i < n; i++)
        {
            printf("%s", direction[safeSequence[i]]);
            if(i != n - 1) printf(" -> ");
        }
        printf("\n\nTraffic Moving Safely...\n");
        printf("Deadlock Avoided Successfully\n");
 
        /* ---- WRITE SEQUENCE TO FILE ---- */
        FILE *f = fopen("sequence.txt", "w");
        if(f != NULL)
        {
            for(i = 0; i < n; i++)
            {
                fprintf(f, "%s", direction[safeSequence[i]]);
                if(i != n - 1) fprintf(f, ",");
            }
            fclose(f);
            printf("\nSequence saved to sequence.txt\n");
        }
        else
        {
            printf("\nWarning: Could not write sequence.txt\n");
        }
        /* -------------------------------- */
    }
    else
    {
        printf("\nDeadlock Detected!\n");
 
        /* Write DEADLOCK to file so HTML can show it */
        FILE *f = fopen("sequence.txt", "w");
        if(f != NULL)
        {
            fprintf(f, "DEADLOCK");
            fclose(f);
        }
    }
 
    return 0;
}