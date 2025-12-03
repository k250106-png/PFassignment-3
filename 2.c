#include <stdio.h>
int  calfuel(int fuel,int planet,int consumption, int recharge, int solarbonus,int totalplanets ){
if (fuel <= 0)
{
    printf("\n fuel exhausted! :(  ");
    return 0;
}
if (totalplanets<planet)
{
    printf("\n wohoo all planet visisted (: ");
    return 1;
}
fuel-=consumption;
fuel+=recharge;
if (planet%4==0)
{
    fuel+=solarbonus;
}
printf("palnet %d with fuel %d \n ", planet,fuel);
return calfuel(fuel,planet+1,consumption,recharge,solarbonus,totalplanets);

}
int main(){
int fuel=100000;
int consumption=5000;
int totplanets;
int planet=1;
int recharge =1000;
int solarbonus=2000;
printf("enter the no.of planets :");
scanf("%d", &totplanets);

calfuel(fuel,planet,consumption,recharge,solarbonus,totplanets);

return 0;
}