#include <iostream>
#include <cstdio>
int main()
{
  char buf[1024]; 
  for(long  i=0; i< 100; ++i)
    {
      sprintf(buf,"%03ld\n",i);
      std::cout << buf ; 
    }
  return  0;  
}
