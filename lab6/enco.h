#ifndef ENCO_H

/* init encoder hardware. Set value to 0.*/
void encoderInit ();
/* return the encoder value , in the range [0,45] */
int encoderValue ();

#endif