#ifndef TYPES_INC
#define TYPES_INC


#include <limits.h>

#include <utility>
// 
// 
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <list>

#define __PI (static_cast<canlib::canlib_float>(4*atan(1)))
#define __DEG2RAD(_co) (static_cast<canlib::canlib_float>((((canlib::canlib_float)_co)/180.0)*__PI))
#define __RAD2DEG(_co) (static_cast<canlib::canlib_float>((((canlib::canlib_float)_co)/__PI)*180.0))

#define __minimum(_a,_b) (((_a)<(_b))?(_a):(_b))
#define __maximum(_a,_b) (((_a)>(_b))?(_a):(_b))
#define __minimum4(_a,_b,_c,_d) (((_a)<(_b))?(((_c)<(_d))?(__minimum((_a),(_c))):(__minimum((_a),(_d)))):(((_c)<(_d))?(__minimum((_b),(_c))):(__minimum((_b),(_d)))))

#define __maximum4(_a,_b,_c,_d) (((_a)>(_b))?(((_c)>(_d))?(__maximum((_a),(_c))):(__maximum((_a),(_d)))):(((_c)>(_d))?(__maximum((_b),(_c))):(__maximum((_b),(_d)))))


#define __abs(a) (((a)>0)?((a)):(-(a)))



	//jeden pokus o zaokrouhlovani - nakonec mi prislo nejlepsi to jednoduse truncnout
//#define floating_to_integer(__co) (((__co)>0)?((((__co) - floating_to_integer(__co)) <= 0.5)?(floating_to_integer(__co)):(floating_to_integer(__co)+1)):((((__co) - floating_to_integer(__co)) >= -0.5)?(floating_to_integer(__co)):(floating_to_integer(__co)+1)))


namespace canlib {
	
	typedef unsigned char small;
	typedef unsigned long canlib_uint;
	typedef long canlib_int; 
	typedef unsigned char canlib_component;
	typedef double canlib_float;
	
	/*int __abs(const int& a);
	float __abs(const float& a);
	double __abs(const double& a);
	char __abs(const char& a);*/

}	
	
#define canlib_INT_MAX LONG_MAX
#define canlib_float_to_int(_fx) (((_fx)<0)?(((_fx) == static_cast<canlib_int>(_fx))?(static_cast<canlib_int>(_fx)):(static_cast<canlib_int>(_fx)-1)):(static_cast<canlib_int>(_fx)))
	//truncnuti, ale u zapornych jeste zmensi o 1 (tvary muzou jit i do "nekreslene" casti plochy)

#define __ceiling(___float) ((((___float) - canlib_int((___float)))>0)?(canlib_int(___float)+1):(canlib_int(___float)))

#endif
