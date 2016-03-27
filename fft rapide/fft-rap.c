#include <stdio.h>

#define N = 2**10
#define PI = PI 3.141592  ;

struct complex {
    double Re ;
    double Im ;
    };

struct Fp {
    double pattern[] = {0} ;
    double Hp ;
    };

struct Fi {
    double pattern[] = {0} ;
    double Hi ;
    };


double fft_rapide(double T_signal[], int indice_init, int taille_porte, double Fe, int nb_freq) {
    int i, j ;
    double H[taille_porte] ;
    if (taille_porte == 1) H[0] = 1 ;
    while (taille_porte > 1) {
        for (i=0 ; i < nb_freq ; i++) {
            for (j=0 ; j < taille_porte ; j++) {
                Fp.Hp[i] += T_signal[2*j]*exp(2*complex(0,1)*PI*2*i*j / taille_porte) ;
                Fi.Hi[i] += T_signal[2*j + 1]*exp(2*complex(0,1)*PI*i*j / taille_porte) ;
                H[i] = Hp[i] + exp(2*complex(0,1)*PI * i / taille_porte) * Hi[i] ;
                Fp.pattern[i] = O ;
                Fi.pattern[i] = 1 ;
            }
        }
        taille_porte = taille_porte/2 ;


            }

        }

        }
    }
    return Hp + Hi ;
}

double separation_pi

double fft(double T_signal[], int taille_porte, double Fe){
    int i, j ;
    double H[taille_porte] ;
    if (taille_porte == 1)  H[0] = 1 ;
    else {
        for (j=0 ; j < taille_porte ; j++){
            for (i=1 ; i<taille_porte, i++) {
                H[j] = T_signal[i]*exp(2*complex(0,1)*PI*i*j / taille_porte)
                }
            }
        }
    return H ;
}

double indices_pairs(double T[]) {
    //on note n la taille du tableau
    int i = n ;
    double rŽsultats[n] ;
    rŽsultat[0] = T[0] ;
    while (i>1) {
        double pairs[i/2] ;
        for (j=0 ; j<i ; j++) { pairs[i] = T[2*j]}
        i = i/2 ;
        }


    }
    rŽsultat[1] = 2**(nombre_boucle)
}
