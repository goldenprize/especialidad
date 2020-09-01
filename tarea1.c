#include<stdio.h>

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
    printf("Value is:\n\r%d", value);
    printf("\n\r");
}

void calculateMCD()
{
    int a = 0, b = 0, q = 0, r = 0; 

    a = getValue();

    b = getValue();

    do
    {
        r = a%b;

        q = ((a - r) * 1/b);

        r = a - (b * q);

        a = b;

        b = r;

        printValue(r);
    }while(r > 0);
}

void main()
{
    calculateMCD();
}