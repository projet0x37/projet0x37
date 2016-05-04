#include <stdio.h>
#include <stdlib.h>
#include <math.h>     /* M_PI   */
#include <string.h>   /* memset */


typedef struct {
  double real;
  double imag;
} cplx;

/*
 * Complexes
 */

void cplx_fprint( FILE *fp, cplx c ) {
  if ( c.real ) {
    fprintf( fp, "%lf", c.real );

    if ( c.imag ) {
      fprintf( fp, "%+lfi ", c.imag );
    }
  }
  else {
    if ( c.imag ) {
      fprintf( fp, "%lfi ", c.imag );
    }
    else {
      fprintf( fp, "0 " );
    }
  }
}

cplx cplx_add( cplx a, cplx b ) {
  cplx c;

  c.real = a.real + b.real;
  c.imag = a.imag + b.imag;

  return c;
}

cplx cplx_sub( cplx a, cplx b ) {
  cplx c;

  c.real = a.real - b.real;
  c.imag = a.imag - b.imag;

  return c;
}

cplx cplx_mul( cplx a, cplx b ) {
  cplx c;

  c.real = a.real * b.real - a.imag * b.imag;
  c.imag = (a.real+a.imag)*(b.real+b.imag) - a.real * b.real - a.imag * b.imag;

  return c;
}

cplx cplx_root( ssize_t k, size_t n ) {
  cplx   r;
  double x = 2*k*M_PI/n;

  r.real   = cos( x );
  r.imag   = sin( x );

  return r;
}

/*
 * Vecteurs
 */

typedef cplx *vec;

#define vec_header(v) (((size_t*)v)-1)
#define vec_size(v)   (*vec_header(v))


vec new_vec( size_t n ) {
  size_t *v = calloc( n, sizeof(size_t)+sizeof(cplx) );

  v[0] = n;

  return (vec)++v;
}

void reset_vec( vec v ) {
  memset( v, 0, sizeof(*v)*vec_size(v) );

  return;
}

void del_vec( vec v ) {
  free( vec_header(v) );
}

void vec_fprint( FILE *fp, vec v ) {
  size_t n = vec_size(v);
  size_t i;

  fprintf( fp, "[ " );

  for ( i = 0; i< n; i++ ) {
    cplx_fprint( fp, v[i] );
  }

  fprintf( fp, "]" );
}


vec new_rand_vec( size_t n ) {
  size_t i;
  vec    v = new_vec( n );

  for ( i = 0; i< n; i++ ) {
    v[i].real = (rand()%1000)/1000.;
    v[i].imag = (rand()%1000)/1000.;
  }

  return v;
}

/*
 * Timer
 */

#include <sys/time.h>

typedef struct timeval timer;

timer init_timer() {
  timer t;
  gettimeofday( &t, NULL );

  return t;
}

size_t tick( timer *t ) {
  timer  now;
  size_t duration;

  gettimeofday( &now, NULL );

  duration = 1e6*t->tv_sec+t->tv_usec;
  duration = 1e6*now.tv_sec+now.tv_usec - duration;

  *t = now;

  return duration;
}

/*
 * Transformée de Fourier lente (formule brute)
 */

void dft( vec in, vec out ) {
  size_t k;
  size_t N = vec_size( in );

  for ( k = 0; k< N; k++ ) {
    size_t n;

    for ( n = 0; n< N; n++ ) {
      out[k] = cplx_add( out[k], cplx_mul( in[n], cplx_root(-n*k,N) ) );
    }
  }

  return;
}

void idft( vec in, vec out ) {
  size_t k;
  size_t N = vec_size( in );

  for ( k = 0; k< N; k++ ) {
    size_t n;

    for ( n = 0; n< N; n++ ) {
      out[k] = cplx_add( out[k], cplx_mul( in[n], cplx_root(n*k,N) ) );
    }

    out[k].real /= N;
    out[k].imag /= N;
  }

  return;
}


/*
 * Transformée de Fourier rapide la plus moisie possible
 * Décimation en temps, base 2
 */

void __fft_rec( vec in, vec out, ssize_t dir ) {
  size_t n = vec_size(in);

  if ( 1 == n ) {
    out[0] = in[0];
    return;
  }
  else {
    size_t  k;
    size_t  nh       = n/2;
    vec     odd_out  = new_vec( nh );
    vec     even_out = new_vec( nh );
    vec     even_in  = new_vec( nh );
    vec     odd_in   = new_vec( nh );

    for ( k = 0; k< nh; k++ ) {
      even_in[k] = in[2*k];
      odd_in[k]  = in[2*k+1];
    }

    __fft_rec( even_in, even_out, dir );
    __fft_rec( odd_in , odd_out , dir );

    for ( k = 0; k< nh; k++ ) {
      cplx    x = cplx_mul( odd_out[k], cplx_root(dir*k, n) );

      out[k]    = cplx_add( even_out[k], x );
      out[k+nh] = cplx_sub( even_out[k], x );
    }

    del_vec( even_in  );
    del_vec( odd_in   );
    del_vec( even_out );
    del_vec( odd_out  );

    return;
  }
}

void fft( vec in, vec out ) {
  __fft_rec( in, out, -1 );
}

void ifft( vec in, vec out ) {
  size_t N = vec_size(in);
  size_t k;

  __fft_rec( in, out, 1 );

  for ( k = 0; k< N; k++ ) {
    out[k].real /= N;
    out[k].imag /= N;
  }
}



/*
 *  On vérifie que ça fonctionne :
 *  - si pas d'argument : test sur un vecteur de taille 8
 *  - si argument entier positif : comparaison des temps DFT vs. FFT
 *    exemple : ./fft 12
 */

#define SZ 8

int main ( int argc, char *argv[] ) {

  if ( 1 == argc ) {

    vec     v = new_rand_vec( SZ );
    vec   out = new_vec( SZ );
    timer now = init_timer();

    fprintf( stdout, "\nVecteur original:\n" );
    vec_fprint( stdout, v );
    fprintf( stdout, "\n" );

    tick( &now );
    dft( v, out );
    fprintf( stdout, "\nLa DFT a pris %lu usec:\n", tick(&now) );
    vec_fprint( stdout, out );
    fprintf( stdout, "\n" );

    tick( &now );
    fft( v, out );
    fprintf( stdout, "\nLa FFT a pris %lu usec:\n", tick(&now) );
    vec_fprint( stdout, out );
    fprintf( stdout, "\n" );

    reset_vec( v );

    tick( &now );
    ifft( out, v );
    fprintf( stdout, "\nLa FFT inverse a pris %lu usec:\n", tick(&now) );
    vec_fprint( stdout, v );
    fprintf( stdout, "\n" );


    del_vec( v );
    del_vec( out );
  }
  else {
    size_t n = 0;
    size_t k = 0;

    if ( !sscanf( argv[1], "%lu", &n ) ) {
      fprintf( stderr, "%s unsigned\n", argv[0] );
      exit( EXIT_FAILURE );
    }

    printf( "Taille\tDFT(usec)\tFFT(usec)\n" );

    for ( k = 0; k< n; k++ ) {
      vec    in = new_rand_vec( 1<<(k+2) );
      vec   out = new_vec( 1<<(k+2) );
      timer now = init_timer();

      dft( in, out );
      printf( "%u\t%lu\t", 1<<(k+2), tick(&now) );
      ifft( out, in );

      tick(&now);
      fft( in, out );
      printf( "\t%lu\n", tick(&now) );

      del_vec( in );
      del_vec( out );
    }
  }

  exit( EXIT_SUCCESS );
}


