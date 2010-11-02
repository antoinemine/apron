#include "vSDP.h"

#include <stdio.h>
#include <stdlib.h>

int infeasCertificate(vSDP* problem, bool dual);	//Checks the possibility of issuing a certificate of infeasibility.

int phaseI(vSDP* problem, bool dual);			//Phase I infeasibility checks.
							//returns 1 in case of detected infesibility (primal/dual according to the value of 'dual' in input)
							//     or 0 otherwise
