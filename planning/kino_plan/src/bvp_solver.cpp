#include "kino_plan/bvp_solver.h"

namespace BVPSolver
{

bool IntegratorBVP::solveDouble()
{
  bool result = calTauStarDouble();
  
  double t2 = tau_star_*tau_star_;
  double t3 = t2*tau_star_;
  coeff_(0, 0) = (2.0*(x0_[0]-x1_[0])+tau_star_*(x0_[3]+x1_[3]))/t3;
  coeff_(0, 1) = -(3.0*(x0_[0]-x1_[0])+tau_star_*(2*x0_[3]+x1_[3]))/t2;
  coeff_(0, 2) = x0_[3];
  coeff_(0, 3) = x0_[0];
  coeff_(1, 0) = (2.0*(x0_[1]-x1_[1])+tau_star_*(x0_[4]+x1_[4]))/t3;
  coeff_(1, 1) = -(3.0*(x0_[1]-x1_[1])+tau_star_*(2*x0_[4]+x1_[4]))/t2;
  coeff_(1, 2) = x0_[4];
  coeff_(1, 3) = x0_[1];
  coeff_(2, 0) = (2.0*(x0_[2]-x1_[2])+tau_star_*(x0_[5]+x1_[5]))/t3;
  coeff_(2, 1) = -(3.0*(x0_[2]-x1_[2])+tau_star_*(2*x0_[5]+x1_[5]))/t2;
  coeff_(2, 2) = x0_[5];
  coeff_(2, 3) = x0_[2];

  return result;
}

bool IntegratorBVP::calTauStarDouble()
{
  VectorXd p(5);
  p[0] = 1;
  p[1] = 0;
  p[2] = (x0_[3]*x0_[3] + x0_[3]*x1_[3] + x1_[3]*x1_[3] 
        + x0_[4]*x0_[4] + x0_[4]*x1_[4] + x1_[4]*x1_[4] 
        + x0_[5]*x0_[5] + x0_[5]*x1_[5] + x1_[5]*x1_[5]) * (-4.0) * rho_;
  p[3] =(- x0_[0]*x0_[3] - x0_[0]*x1_[3] + x1_[0]*x0_[3] 
        + x1_[0]*x1_[3] - x0_[1]*x0_[4] - x0_[1]*x1_[4] 
        + x1_[1]*x0_[4] + x1_[1]*x1_[4] - x0_[2]*x0_[5] 
        - x0_[2]*x1_[5] + x1_[2]*x0_[5] + x1_[2]*x1_[5]) * 24.0 * rho_;
  p[4] =(- x0_[0]*x0_[0] + 2.0*x0_[0]*x1_[0] - x1_[0]*x1_[0] 
        - x0_[1]*x0_[1] + 2.0*x0_[1]*x1_[1] - x1_[1]*x1_[1] 
        - x0_[2]*x0_[2] + 2.0*x0_[2]*x1_[2] - x1_[2]*x1_[2]) * 36.0 * rho_;
  std::set<double> roots = RootFinder::solvePolynomial(p, DBL_EPSILON, DBL_MAX, FLT_EPSILON);
  
  bool result = false;
  double tau = DBL_MAX;
  double cost = DBL_MAX;
  for (const double& root : roots) 
  {
    double t1 = x0_[0] - x1_[0];
    double t2 = x0_[1] - x1_[1];
    double t3 = x0_[2] - x1_[2];
    double t4 = x0_[3] + x1_[3];
    double t5 = x0_[4] + x1_[4];
    double t6 = x0_[5] + x1_[5];
    double t7 = t1*t1 + t2*t2 + t3*t3;
    double t8 = t4*t4 + t5*t5 + t6*t6 - x0_[3]*x1_[3] - x0_[4]*x1_[4] - x0_[5]*x1_[5];
    double t9 = t1*t4 + t2*t5 + t3*t6;
    
    double current = root + rho_*(t7*12/root/root/root + t8*4/root + t9*12/root/root);
    if (current < cost) 
    {
      tau = root;
      cost = current;
      result = true;
    }
  }
  tau_star_ = tau;
  cost_star_ = cost;
  return result;
}

bool IntegratorBVP::solveTriple()
{
  bool result = calTauStarTriple();
  
  double t2 = tau_star_*tau_star_;
  double t3 = tau_star_*t2;
  double t4 = tau_star_*t3;
  double t5 = tau_star_*t4;
  coeff_(0, 0) = -(12*x0_[0] + 6*tau_star_*x0_[3] + t2*x0_[6] - 12*x1_[0] + 6*tau_star_*x1_[3] - t2*x1_[6])/(2*t5); 
  coeff_(1, 0) = -(12*x0_[1] + 6*tau_star_*x0_[4] + t2*x0_[7] - 12*x1_[1] + 6*tau_star_*x1_[4] - t2*x1_[7])/(2*t5); 
  coeff_(2, 0) = -(12*x0_[2] + 6*tau_star_*x0_[5] + t2*x0_[8] - 12*x1_[2] + 6*tau_star_*x1_[5] - t2*x1_[8])/(2*t5);
  coeff_(0, 1) = -(-30*x0_[0] - 16*tau_star_*x0_[3] - 3*t2*x0_[6] + 30*x1_[0] - 14*tau_star_*x1_[3] + 2*t2*x1_[6])/(2*t4);
  coeff_(1, 1) = -(-30*x0_[1] - 16*tau_star_*x0_[4] - 3*t2*x0_[7] + 30*x1_[1] - 14*tau_star_*x1_[4] + 2*t2*x1_[7])/(2*t4);
  coeff_(2, 1) = -(-30*x0_[2] - 16*tau_star_*x0_[5] - 3*t2*x0_[8] + 30*x1_[2] - 14*tau_star_*x1_[5] + 2*t2*x1_[8])/(2*t4); 
  coeff_(0, 2) = -(20*x0_[0] + 12*tau_star_*x0_[3] + 3*t2*x0_[6] - 20*x1_[0] + 8*tau_star_*x1_[3] - t2*x1_[6])/(2*t3); 
  coeff_(1, 2) = -(20*x0_[1] + 12*tau_star_*x0_[4] + 3*t2*x0_[7] - 20*x1_[1] + 8*tau_star_*x1_[4] - t2*x1_[7])/(2*t3); 
  coeff_(2, 2) = -(20*x0_[2] + 12*tau_star_*x0_[5] + 3*t2*x0_[8] - 20*x1_[2] + 8*tau_star_*x1_[5] - t2*x1_[8])/(2*t3); 
  coeff_(0, 3) = x0_[6]/2; 
  coeff_(1, 3) = x0_[7]/2; 
  coeff_(2, 3) = x0_[8]/2;
  coeff_(0, 4) = x0_[3]; 
  coeff_(1, 4) = x0_[4];
  coeff_(2, 4) = x0_[5]; 
  coeff_(0, 5) = x0_[0]; 
  coeff_(1, 5) = x0_[1]; 
  coeff_(2, 5) = x0_[2];

  return result;
}

bool IntegratorBVP::calTauStarTriple()
{
  VectorXd p(5);
  p[0] = 35 + rho_*(3*x0_[6]*x0_[6] + 3*x0_[7]*x0_[7] + 3*x0_[8]*x0_[8] 
                    + x0_[6]*x1_[6] + 3*x1_[6]*x1_[6] + x0_[7]*x1_[7]
                    + 3*x1_[7]*x1_[7] + x0_[8]*x1_[8] + 3*x1_[8]*x1_[8]);
  p[1] = 0;
  p[2] = -6*rho_*(32*x0_[3]*x0_[3] + 32*x0_[4]*x0_[4] + 32*x0_[5]*x0_[5]
                + 5*x0_[0]*x0_[6] + 5*x0_[1]*x0_[7] + 5*x0_[2]*x0_[8] 
                - 5*x0_[6]*x1_[0] - 5*x0_[7]*x1_[1] - 5*x0_[8]*x1_[2] 
                + 36*x0_[3]*x1_[3] + 32*x1_[3]*x1_[3] + 36*x0_[4]*x1_[4] 
                + 32*x1_[4]*x1_[4] + 36*x0_[5]*x1_[5] + 32*x1_[5]*x1_[5] 
                - 5*x0_[0]*x1_[6] + 5*x1_[0]*x1_[6] - 5*x0_[1]*x1_[7] 
                + 5*x1_[1]*x1_[7] - 5*x0_[2]*x1_[8] + 5*x1_[2]*x1_[8]);
  p[3] = -1200*rho_*(x0_[2]*x0_[5] - x0_[3]*x1_[0] - x0_[4]*x1_[1] - x0_[5]*x1_[2] 
                  - x1_[0]*x1_[3] + x0_[0]*(x0_[3] + x1_[3]) - x1_[1]*x1_[4] 
                  + x0_[1]*(x0_[4] + x1_[4]) + x0_[2]*x1_[5] - x1_[2]*x1_[5]);
  p[4] = -1800*rho_*(x0_[0]*x0_[0] + x0_[1]*x0_[1] + x0_[2]*x0_[2] 
                - 2*x0_[0]*x1_[0] + x1_[0]*x1_[0] - 2*x0_[1]*x1_[1] 
                  + x1_[1]*x1_[1] - 2*x0_[2]*x1_[2] + x1_[2]*x1_[2]);
  std::set<double> roots = RootFinder::solvePolynomial(p, DBL_EPSILON, DBL_MAX, FLT_EPSILON);
  
  bool result = false;
  double tau = DBL_MAX;
  double cost = DBL_MAX;
  
  for (const double& root : roots) 
  {
    double current = (1/(35*root*root*root))
    *(600*rho_*((x0_[0] - x1_[0])*(x0_[0] - x1_[0]) 
              + (x0_[1] - x1_[1])*(x0_[1] - x1_[1]) 
              + (x0_[2] - x1_[2])*(x0_[2] - x1_[2]))
    + 600*rho_*root*((x0_[0] - x1_[0])*(x0_[3] + x1_[3]) 
                      + (x0_[1] - x1_[1])*(x0_[4] + x1_[4]) 
                      + (x0_[2] - x1_[2])*(x0_[5] + x1_[5]))
    + 6*rho_*root*root*(32*x0_[3]*x0_[3] + 32*x0_[4]*x0_[4] + 36*x0_[3]*x1_[3] 
                              + 36*x0_[4]*x1_[4] + 32*(x1_[3]*x1_[3] + x1_[4]*x1_[4]) 
                              + 4*(8*x0_[5]*x0_[5] + 9*x0_[5]*x1_[5] + 8*x1_[5]*x1_[5])
                              + 5*(x0_[0] - x1_[0])*(x0_[6] - x1_[6]) 
                              + 5*(x0_[1] - x1_[1])*(x0_[7] - x1_[7]) 
                              + 5*(x0_[2] - x1_[2])*(x0_[8] - x1_[8]))
    + 2*rho_*root*root*root*(11*x0_[3]*x0_[6] + 11*x0_[4]*x0_[7] 
                                      + 11*x0_[5]*x0_[8] + 4*x0_[6]*x1_[3] 
                                      + 4*x0_[7]*x1_[4] + 4*x0_[8]*x1_[5] 
                                      - 4*x0_[3]*x1_[6] - 11*x1_[3]*x1_[6] 
                                      - 4*x0_[4]*x1_[7] - 11*x1_[4]*x1_[7] 
                                      - 4*x0_[5]*x1_[8] - 11*x1_[5]*x1_[8])
    + root*root*root*root*(35 + rho_*(3*x0_[6]*x0_[6] + 3*x0_[7]*x0_[7] 
                                                      + x0_[6]*x1_[6] + x0_[7]*x1_[7] 
                                                      + 3*(x0_[8]*x0_[8] + x1_[6]*x1_[6] + x1_[7]*x1_[7])
                                                      + x0_[8]*x1_[8] + 3*x1_[8]*x1_[8])));
    if (current < cost) 
    {
      tau = root;
      cost = current;
      result = true;
    }
  }

  tau_star_ = tau;
  cost_star_ = cost;
  return result;
}

}