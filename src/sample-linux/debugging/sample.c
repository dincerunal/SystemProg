#include <stdio.h>

int main(int argc, char **argv)
{ 
    int val;    
    int *p = &val; 
    
    val = 34;
  
    {
      int val;
      
      val = 40;
      
      *p = 30;
      
      printf("val=%d\n", val);
      
    }
    
    printf("val=%d\n", val);
    
    
    return 0;
}