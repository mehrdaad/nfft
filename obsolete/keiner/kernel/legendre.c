#include "legendre.h"

inline double alpha_al (int k, int n)
{ 
  if (k == -1 || (k == 0 && (n %2) == 1))
  {
    return (0.0); 
  }
  else if (k < n) 
  {
    return k%2==0?-1.0:1.0;
  }  
  else 
  {
    return (2.0*k+1.0) / sqrt ((k-n+1.0) * (k+n+1.0));	      
  }
}

inline double beta_al (int k, int n)
{
  if (0 <= k && k < n)
  {
    return (1.0);
  }
  else
  {
    return (0.0);
  }
}

inline double gamma_al (int k, int n)
{ 
  static int i;
  static double result;
  
  if (k == -1) 
  {
    /* Constant is ((2n)!)^(1/2) / (2^n n!). */
    result = 1.0;
    for (i = 1; i <= n; i++)
    {  
      result *= (n+i)/(4.0*i);                                      
    }  
    return (sqrt(result));
  }
  else if (k <= n) 
  {
    return (0.0);
  }
  else 
  {
    return (-sqrt(((double)(k-n)*(k+n))/(((k-n+1.0)*(k+n+1.0))))); 
  }
}							

inline void init_al_coeff(double *alpha, double *beta, double *gamma,
 int l, int m, int n)
{
  int i;
  
  for (i = 0; i < m; i++)
  { 
    alpha[i+1] =  alpha_al(l+i,n);
    beta[i+1] = beta_al(l+i,n);
    gamma[i+1] = gamma_al(l+i,n);
  }
}

inline void eval_al(double *x, double *y, int size, int m, double *alpha, 
  double *beta, double *gamma)
{
  /* Evaluate the associated Legendre polynomial P_{m,nleg} (l,x) for the vector 
   * of knots  x[0], ..., x[size-1] by the Clenshaw algorithm
   */
  int i,j;
  double a,b,x_val_act,a_old;
  double *x_act, *y_act;  
  double *alpha_act, *beta_act, *gamma_act;
  
  /* Traverse all nodes. */
  x_act = x;
  y_act = y;
  for (i = 0; i < size; i++)
  {
    a = 1.0;
    b = 0.0;
    x_val_act = *x_act;
    
    if (m == 0)
    {  
      *y_act = 1.0;
    }
    else
    {
      alpha_act = &(alpha[m]);
      beta_act = &(beta[m]);
      gamma_act = &(gamma[m]);
      for (j = m; j > 1; j--)
      {
        a_old = a;
        a = b + a_old*((*alpha_act)*x_val_act+(*beta_act));		        
	       b = a_old*(*gamma_act);
        alpha_act--;
        beta_act--;
        gamma_act--;
      }
      *y_act = (a*((*alpha_act)*x_val_act+(*beta_act))+b);                  
    }
    x_act++;
    y_act++;
  }
}

inline bool eval_al_thresh(double *x, double *y, int size, int m, double *alpha, 
  double *beta, double *gamma, double threshold)
{
  /* Evaluate the associated Legendre polynomial P_{m,nleg} (l,x) for the vector 
   * of knots  x[0], ..., x[size-1] by the Clenshaw algorithm
   */
  int i,j;
  double a,b,x_val_act,a_old;
  double *x_act, *y_act;
  double *alpha_act, *beta_act, *gamma_act;
  
  /* Traverse all nodes. */
  x_act = x;
  y_act = y;
  for (i = 0; i < size; i++)
  {
    a = 1.0;
    b = 0.0;
    x_val_act = *x_act;
    
    if (m == 0)
    {  
     *y_act = 1.0;
    }
    else
    {
      alpha_act = &(alpha[m]);
      beta_act = &(beta[m]);
      gamma_act = &(gamma[m]);
      for (j = m; j > 1; j--)
      {
        a_old = a;
        a = b + a_old*((*alpha_act)*x_val_act+(*beta_act));		        
	       b = a_old*(*gamma_act);
        alpha_act--;
        beta_act--;
        gamma_act--;
      }
      *y_act = (a*((*alpha_act)*x_val_act+(*beta_act))+b);                  
      if (fabs(*y_act) > threshold)
        return true;
    }
    x_act++;
    y_act++;
  }
  return false;
}


void alpha_al_all(double *alpha, int N)
{
  int i,j;
  double *alpha_act = alpha;
  for (i = 0; i <= N; i++)
  {
    for (j = 0/*i*/; j <= N; j++)
    {
      *alpha_act = alpha_al(j-1,i); 
      alpha_act++;
    }  
  }  
}

void beta_al_all(double *alpha, int N)
{
  int i,j;
  double *alpha_act = alpha;
  for (i = 0; i <= N; i++)
  {
    for (j = 0/*i*/; j <= N; j++)
    {
      *alpha_act = beta_al(j-1,i); 
      alpha_act++;
    }  
  }  
}

void gamma_al_all(double *alpha, int N)
{
  int i,j;
  double *alpha_act = alpha;
  for (i = 0; i <= N; i++)
  {
    for (j = 0/*i*/; j <= N; j++)
    {
      *alpha_act = gamma_al(j-1,i); 
      alpha_act++;
    }  
  }  
}

void gamma_al_m1_all(double *alpha, int N)
{
  int i;
  double *alpha_act = alpha;
  for (i = 0; i <= N; i++)
  {
    *alpha_act = gamma_al(-1,i); 
    alpha_act++;
  }  
}
