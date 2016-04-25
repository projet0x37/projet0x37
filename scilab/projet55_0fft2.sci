    function[] = projet55_0fft(c,a,L,n,u,nb)
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
    fig1=figure(1)
    fig0=figure(0)
    fig2=figure(2)
    fig3=figure(3)
    close(fig1)
    close(fig2)
    close(fig0)
    close(fig3)
    
    
    N=d
    
    [y,fs,bits] = wavread(c,[N*(n-1)+1,N*n])
    
    Tf = N*n/fs
    To = (N*(n-1))/fs
    t = [To:1/fs:Tf-1/fs]
    disp(Tf,"fin",To,"debut") // Renvoie entre quand et quand la porte commence et se termmine ( en secondes)
    
    x = [0:1:N/2-1] // vecteur abcisse fréquentielle (entiers)
    xfft = x*fs/N // vecteur abcisse fréquentielle (normale)
    
    L = floor(N/2)
    u = u*N/fs // conversion de u en fréquence entière
    nmax = floor(log(6000/u)/log(4/3))+1   //plus grand n tel que L soit inférieur à u0*(4/3)^n ou n est le nb d'itération de la boucle
    
    
    //Fenêtre de Hamming w
    w = [0:N-1]
    w = (0.54-0.46*cos(2*%pi*w/N))
    
    // Passage du signal par la fenêtre de Hamming
    y(1,:) = y(1,:).*w
    
    //Affichage temporel du signal
    figure(0)
    subplot(2,1,1)
    plot(t,y(1,:))
    
    //Calcul des transformées de Fourier du signal
    yfft1 = fft(y(1,:))
    yfft1 = yfft1(1:L) // limitation du spectre à L
    yb1 = abs(yfft1).^2 // Bien prendre le module au carré
    xfft = xfft(1:L) // limitation des abcisses à L
    
    //Affichage du spectre figure 0 et 1
    subplot(2,1,2)            //affichage du spectre abscisses freq (normale)
    plot(yb1)

        
    //Suppression du bruit et affichage sur figure 2
    figure(2)
    subplot(2,1,1)
    z=noisesup(1.5,100,N,fs,yb1,50,7000,xfft)
    
    //affichage bande passante
    
    m0=0
    m2=0
    [b,m0,m2]=BW(100,N,fs,L,u*(4/3)**nb)
//    subplot(3,1,1)
//    plot(b)
    
    //Affichage Lb
//    figure(1)
//    subplot(2,1,1)
//    zb=z.*b
//    plot(zb)
//    Kb=floor(m2)-m0 +1
//    Lb=lbvector(L,Kb,zb,m0,u,N,fs)
//    subplot(2,1,2)
//    plot(xfft,Lb)
    
    //affichage vecteur Ltot
    lv=lvector(100,nmax,z,N,fs,u,L)
    figure(3)
    
    plot(xfft,lv)
    
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
            if k>=kmin/2 then
                m(k)=mean(y(ceil(k-kmin/2):round(k+kmin/2)));
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
    if k1>k0 & k1<s then // évite d'avoir des erreurs, renvoie y à zéros sinon
        
        for i = k0 : k1
            g=g+x(i)**(1/3)
        end
        g=g/(k1-k0+1)
        g=g**3
        
        disp(g,"g")
        
        y=log(1+x/g)
        
        m=movingaverage(ratio,bandfmin,N,fs,y)
        // affichage du spectre Y(k) et de la moyenne courante m sur le méme grahe ( ici sur la figure 2 au milieu)
        plot(xfft,m,'r')
        plot(xfft,y,'g')
        for i = 1:s
            y(i)=max(0,y(i)-m(i))
        end
        
        // affichage du spectre final Z(k) en bas de la figure 2
        subplot(2,1,2)
        plot(xfft,y)
        
    end
    
    endfunction
        
        function [b,m0,m2]=BW(Bmin,N,fs,l,kb) // ok
            b = zeros(1,l)
            kmin = Bmin*N/fs
            kb=round(kb)
            
            if kb*2/3 > kmin then
                m0 = kb
                m1 = kb*4/3
                m2 = kb*5/3
                b1 = m0/(m0-m1)
                a1 = 1/(m1-m0)
                b2 = m2/(m2-m1)
                a2 = 1/(m1-m2)
                i = m0
                while i <= m1 & i<=l
                    b(i) = a1*i+b1;
                    i=i+1;
                end
                while i <= m2 & i<=l
                    b(i) = a2*i+b2;
                    i=i+1
                end
            else
                m0 = kb
                m1 = kb + kmin/2
                m2 = kb + kmin
                b1 = m0/(m0-m1)
                a1 = 1/(m1-m0)
                b2 = m2/(m2-m1)
                a2 = 1/(m1-m2)
                i = m0
                while i <= m1 & i<=l
                    b(i) = a1*i+b1;
                    i=i+1;
                end
                while i < m2 & i<=l
                    b(i) = a2*i+b2;
                    i=i+1
                end
                
            end
            m2=i-1// afin d'avoir un Kb correct
        endfunction
        
        
        
        function L=lvector(Bmin,nmax,z,N,fs,u,l) 
            L=zeros(1,l)
            kb=u
            i=0
            m0=0
            m2=0
            while i < nmax
                if i== 0 then
                    [b,m0,m2]=BW(Bmin,N,fs,l,kb)
                    zb=z.*b
                    Kb=floor(m2)-m0+1
                    Lb=lbvector(l,Kb,zb,m0,u,N,fs)
                    L=L+Lb
                    i=i+1
                else
                    kb = kb*4/3
                    [b,m0,m2]=BW(Bmin,N,fs,l,kb)
                    zb=z.*b
                    Kb=floor(m2)-m0+1
                    Lb=lbvector(l,Kb,zb,m0,u,N,fs)
                    L=L+Lb
                    i=i+1
                end
            end
            
        endfunction
        
        function Lb=lbvector(l,Kb,zb,kb,u,N,fs)
            Lb=zeros(1,l)
            n0 = round(u)
            
            n1 = Kb -1
            lb= kb +n1
            for n = n0:n1
                m0=round(ceil(kb/n)*n)-kb
                delta = lb*(sqrt(1+0.01*((lb/n)**2-1))-1)
                m1=m0+delta
                if m1> m0+n-1 then
                    m0=0
                    m1=n-1
                end   
                
                Lb(n)=0
                for m = m0:m1
                    J = floor((Kb-m-1)/n)+1
                    if J <=0 then
                        J=1
                    end
                    c = 0.75/J + 0.25
                    Ln=0
                    i=0
                    while i <= J-1 & kb+m+n*i<=l
                        Ln=Ln+c*zb(kb+m+n*i)
                        i=i+1
                    end
                    if Lb(n)<Ln then
                        Lb(n) = Ln
                    end
                end
            end
            
            h=1
            k0=floor((kb+Kb)/(h+1))
            if k0<kb then
                k0=kb
            end
            k1=kb+Kb-1
            while k0<=k1
                for k=k0:k1
                    n=round(k/h)
                    if k<l then

                    if Lb(n)<zb(k) then
                        Lb(n)=zb(k)
                    end
                    end
                end
                h=h+1
                k0=ceil((kb+Kb)*h/(h+1))
                if k0 < kb then 
                    k0=kb
                end
                k1=floor((kb-1)*h/(h-1))
                if k1 > kb+Kb then
                    k1= kb+Kb
                end
            end
        endfunction


