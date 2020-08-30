
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
    int a = 0, a_ini = 0, b = 0, q = 0, r = 0; 

    a_ini = getValue();

    b = getValue();

    r = a_ini%b;

    while(r > 0)
    {
        r = a_ini%b;

        q = ((a_ini - r) * 1/b);

        b = r;

        a = b * q + r;

        printf("\n\rValue for R is: %d\n\r", r);
    }
}

void main()
{
    calculateMCD();
}