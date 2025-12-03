#include <stdio.h>
int loanfunc(float loan , float  interestrate,int yrs,int installment){
if (yrs==0 || loan <=0)
{
    return 0;
}

loan=loan+(loan*interestrate);
float pay=(loan>=installment)?installment:loan;
loan-=pay;

printf("year %d loan remaining is :%.2f\n", 4-yrs,loan);
return pay+loanfunc(loan,interestrate,yrs-1,installment);
}
int main(){

int installment=40000;
float interestrate=0.05;
int yrs=3;
float loan =100000;


float total=loanfunc(loan,interestrate,yrs,installment);
printf("\ntotal payment%.2f in %d yrs \n ", total,yrs);


return 0;
}
