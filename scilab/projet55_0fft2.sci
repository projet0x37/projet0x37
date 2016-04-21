    function[] = projet55_0fft(c,a,L,n,u)
    //script pour l'affichage de la transformée de Fourier rapide d'un fichier wav
    //selon un canal
    // Variables :
    // c pour le nom du fichier wav
    // n pour le debut de le porte
    // a pour choisir la largeur de la porte
    // L pour limiter le spectre entre 0 et L
    // u fréquence du début de la première bande passante
    //Pour choisir la largeur des portes  en supposant un échantillonnage usuel à 44100 Hz
    //1 >> 2**12  92 ms 
    //2 >> 2**13  185 ms
    //3 >> 2**14  371 ms
    //4 >> 2**15  743 ms
    select a
    case 1 then
        d=2**12
    case 2 then
        d=2**13
    case 3 then
        d=2**14
    case 4 then
        d=2**15
    else d=2**15
    end
    N=d
    [y,fs,bits] = wavread(c,[N*(n-1)+1,N*n])
    Tf = N*n/fs
    To = (N*(n-1))/fs
    t = [To:1/fs:Tf-1/fs]
    disp(Tf,"fin",To,"debut") // Renvoie entre quand et quand la porte commence et se termmine ( en secondes)
    x = [0:1:N/2-1] // vecteur abcisse fréquentielle (entiers)
    xfft = x*fs/N // vecteur abcisse fréquentielle (normale)
    L = floor(L*N/fs)
    u = u*N/fs // pas besoin de prendre la partie entière contrairement à L qui est utilisé pour tracer 
    nmax = floor(log(L/u)/log(4/3))   //plus grand n tel que L soit inférieur à u0*(4/3)^n ou n est le nb d'itération de la boucle
    
    
    //Fenêtre de Hamming w
    w = [0:N-1]
    w = (0.54-0.46*cos(2*%pi*w/N))
    // Passage du signal par la fenêtre de Hamming
    y(1,:) = y(1,:).*w
    
    clf()
    
    //plot(y(1,:),'r')
    //plot(w)
    
    //Affichage temporel du signal
    subplot(2,1,1)
    plot(t,y(1,:))
    
    //Calcul des transformées de Fourier du signal
    yfft1 = fft(y(1,:))
    yfft1 = yfft1(1:L) // limitation du spectre à L
    yb1 = abs(yfft1).^2 // Bien prendre le module au carré
    xfft = xfft(1:L) // limitation des abcisses à L
    
    //Affichage du spectre figure 0 et 1
    subplot(2,1,2)            //affichage du spectre abscisses freq (normale)
    plot(xfft,yb1)
    figure(2)
    subplot(3,1,1)            //affichage du spectre abscisses freq (normale)
    plot(xfft,yb1)
    //Suppresion du bruit et affichage sur figure 2
    yb1=noisesup(1.5,100,N,fs,yb1,50,6000)
//    
//    //Affichage des bandes passantes
//    figure(1)
//    subplot(nmax+3,1,1)              //affichage du spectre abscisses freq entiers
//    plot(x(1:L),yb1)
//    
//    
//    TLb = zeros(1,nmax+2)  //initialisation du tableau de Lb (+2 pour ajourter le Lb correspondant au triangle à 50 Hz et eventuellement un autre)
//    BWi = BWin(fs,L,N)
//    yb11=yb1.*BWi(1:L)     //Calcul du Zb initial càd celui correspondant à la                             BW de 100Hz
//    subplot(nmax+3,1,2)
//    plot(x(1:L),BWi(1:L))
//    subplot(nmax+3,1,3)
//    plot(x(1:L),yb11)
//    for i = 1:1:nmax
//        kb=floor(floor((100*(2**12))/44100)*(4/3)**i)
//        BW = BWb(fs,L,N,kb)
//        yb11=yb1.*BW(1:L)    //Calcul du Zb
//        subplot(nmax+3,1,2)
//        plot(x(1:L),BW(1:L))
//        subplot(nmax+3,1,3+i)
//        plot(x(1:L),yb11)
//    end
    
    
    
    
    
    mclose('all')
    
    
    endfunction


function m=movingaverage(ratio,bandfmin,N,fs,y)
    s=length(y)
    m=zeros(1,s)
    
    kmin=fmin*N/fs
    l=0
    for k = 1:s
        if ratio*k<=kmin then
            l=round(kmin)
            if k>=kmin then
                m(k)=mean(y(round(k-kmin/2):round(k+kmin/2)));
            else
                m(k)=mean(y(1,round(k*3/2)));
            end
        else
            if s-k<=ratio*k then
                m(k)=mean(y(round(k-(s-k)/2):s));
            else
                m(k)=mean(y(round(k*(1-ratio/2)):round(k*(1+ratio/2))))
            end
        end
    end
    
endfunction


function y=noisesup(ratio,bandfmin,N,fs,x,fmin,fmax,xfft)
    s=length(x)
    y=zeros(1,s)
    g=0
    k0=fmin*N/fs
    k1=fmax*N/fs
    if k1>k0 & k1<s then // évite d'avoir des erreurs
        for i = k0 : k1
            g=g+x(i)**(1/3)
        end
        g=g/(k1-k0+1)
        g=g**3
        disp(g);
        y=log(1+x/g)
        
        m=movingaverage(ratio,bandfmin,N,fs,y)
        subplot(3,1,2)
        plot(xfft,m,'r')
        plot(xfft,y,'g')
        for i = 1:s
            y(i)=max(0,y(i)-m(i))
        end
        subplot(3,1,3)
        plot(xfft,y)
    end
    
    endfunction


    function BW=BWin(fs,L,N)
        BW=zeros(1,N/2)
        m = round(50*N/fs)
        B = round(100*N/fs)+1
        M = m  + B
        m1 = round((m+M)/2)
        m2 = m1+1
        if modulo(B,2) == 0 then 
            m2=m1;
        end
        b1 = m/(m-m1)
        a1 = (1-b1)/m1
        b2 = M/(M-m2)
        a2 = (1-b2)/m2
        for i = m:1:m1
            BW(i)= a1*i+b1;
        end
        for i = m2:1:M
            BW(i)= a2*i + b2;
        end
        //x=[0:1:N/2-1]
        //plot(x,BW)
        endfunction
        
        function BW=BWb(fs,L,N,kb)
            BW = zeros(1,N/2)
            m0 = kb
            m1 = kb*4/3
            m2 = kb*5/3
            a1 = 3/kb
            b1= -3
            a2 = -3/kb
            b2 = 5
            i = m0
            while i <= m1
                BW(i) = a1*i+b1;
                i=i+1;
            end
            while i <= m2
                BW(i) = a2*i+b2;
                i=i+1
            end
            //x=[1:1:N/2]
            //plot(x,BW)
        endfunction
