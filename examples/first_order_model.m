% First order model discretization

K = 1;     % static gain
tau = 0.3; % time constant 
Te = 0.01; % sampling time [s]

c = exp((-1/tau) * Te) 
a1 = c 
b1 = (1 - c)*K

