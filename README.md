# One Spin LLG Solver
 Solves LLG using RK4, RK2 or Euler methods.
 To change the integration method, go to `main.c` and in the `#include "method"` change `method` for the wanted integration method.
 The animation is created by `cria_anim.py`. To choose the animation duration, go to `cria_anim.py` and change the call method `faz(duration)` with the desired duration.
 In the animation, the red arrow is the external Magnetic Field and the black arrow is the Spin vector (magnetization vector over magnetization norm).
 The external field changes over time, and to set how it changes, go to `method.c` where `method` is the integration method you're using and change the function `void* calccampo(void *k)`. The time parameter is `k_`. Example:
 lets say the external field is H=(0.5t, t, 0), then the function will be:
 `void* calccampo(void *k){
      double k_ = *(double*) k;
      H->x = 0.5 * k_;
      H->y = k_;
      H->z = 0.0;
      return NULL;
 }`
 By default, the external field is set to H=(0, 0, 1).
 To set the parameters of simulation, you can change directly in `method.c` right at the beggining of the file. The `corte` parameter is just a cut in the out file in order to decrease the file size.`n` is the number of iteractions, `tmax` is the simulation final time, `tmin` is the simulation start time. Personally, i think these `tmax` and `tmin` are useless, but because the external Magnetic Field can change over time, maybe this will be used. `lambda` is the damping term.
 You can set the parameters when calling the executable choosing one of the following:
 `gcc main.c -lpthread`
 `a.exe n tmin tmax lambda corte`
 `a.exe n tmin tmax lambda` 
 `a.exe n tmin tmax`
 `a.exe n tmax`
 `a.exe n`
 If you call just `a.exe` the parameters will be the ones in the `method.c`.
 The `out.dat` file is the Sping vector coordinates, the `field.dat` file is the external Magnetic Field.
 Important: you'll need the `<pthread.h>` header in order for this code work.
