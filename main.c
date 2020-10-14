/*
 * main.c
 *
 *  Created on: 30 ago. 2020
 *      Author: Ernesot Ramirez Peña
 *      Ingeniera de Software.
 */

#include <stdio.h>


void main(){
	int a,b,q,r,temp;

	printf ("\r\n Please the following a = b*q + r \r\n");
	printf ("\r\n Please provide a: \r\n");
	a = get_char ();
	printf ("\r\n Please provide b: \r\n");
	b = get_char ();
	q = a / b;
	r = a % b;
	printf ("q= %u");
}
