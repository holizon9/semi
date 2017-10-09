

double rand_normal( double mu, double sigma ){
    double z=sqrt( -2.0*log(Uniform()) ) * sin( 2.0*M_PI*Uniform() );
  return mu + sigma*z;
}