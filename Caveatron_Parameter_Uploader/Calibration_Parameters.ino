//Caveatron Calibration Parameters
//Serial Number: xxxxx
//Date: 2018-xx-xx

extern char serialNumber[] = "xxxxx";

extern char hardwareCode[] = "11121011110";

extern char screenCal[3][11] = {"03D300A2UL","00238F28UL","0013F1DFUL"};

extern float accCal[4][3] = {
  {-2.13659E-02,7.34896E-03,-2.78281E-02},
  {6.34247E-05,1.84463E-08,3.66437E-07},
  {1.38812E-08,6.26337E-05,-3.47865E-07},
  {-3.38732E-08,2.60433E-07,6.28441E-05}
};

extern float magAlignCalNoLid[8] = {-75,100,0,2.9,55,75,0,3.4};

extern float magAlignCalLidarXV[8] = {-45,80,0,2.9,65,30,0,3.4};

extern float magAlignCalLidarSW[8] = {-60,140,0,2.7,50,90,0,3.6};

extern float magAlignCalLidarRP[8] = {-60,140,0,2.7,50,90,0,3.6};

extern float magCalNoLid[4][3] = {
  {159.7422028,-529.4240723,-852.3153687},
  {0.8084373,-0.0899807,0.0264916},
  {-0.0899807,0.9351358,0.0089281},
  {0.0264916,0.0089281,0.9957898}
};

extern float magCalLidarXV[4][3] = {
  {2358.0046387,-1308.2824707,-408.2989502},
  {0.8149756,-0.0931167,0.0251307},
  {-0.0931167,0.9354818,0.0106639},
  {0.0251307,0.0106639,0.9963638}
};

extern float magCalLidarSW[4][3] = {
  {2328.1503906,-1157.1542969,-373.3346863},
  {0.8116457,-0.0914898,0.0263052},
  {-0.0914897,0.9353983,0.0116162},
  {0.0263052,0.0116162,0.9962596}
};

extern float magCalLidarRP[4][3] = {
  {2328.1503906,-1157.1542969,-373.3346863},
  {0.8116457,-0.0914898,0.0263052},
  {-0.0914897,0.9353983,0.0116162},
  {0.0263052,0.0116162,0.9962596}
};

extern float lidarXVOrientCal[1] = {304};

extern float lidarSWOrientCal[1] = {0};

extern float lidarRPOrientCal[1] = {0};

extern float LRFRangeCal[1] = {0};
