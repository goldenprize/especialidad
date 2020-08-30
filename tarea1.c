
#include<stdio.h>
//#include<


int getValue()
{
    int value = 0; 

    printf("\n\r");
    printf("Please insert the value to calculate the MCD:\n\r");
    scanf("%d", &value);
    printf("\n\r");

    return value;
}

void printValue(int value)
{
    printf("\n\r");
    printf("Given value is:\n\r %d", value);
    printf("\n\r");
}

void calculateMCD()
{
    int a = 0, b = 0, q = 0, r = 0; 

    a = getValue();

    b = getValue();

    r = a%b;

    printValue(r);

}


void main()
{
    calculateMCD();
}