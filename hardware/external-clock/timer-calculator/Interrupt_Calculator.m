f_osc = 1000000;       % 1 MHz Oscillator
t_instr = 4/f_osc;       % 4 per instruction on PIC

prescaler = [1 2 4 8 16 32 64 128 256];
timer = 1:2^16;

t_des = 0.188600031806 - 54 * t_instr;  % desired time minus time required
                                        % by interrupt intstructions
t_des = t_des * ones(1,length(timer));  % desired time (with gear)
% 3.365785183 would be measurement with 256 tick reso. directly on axis


err_min = 99999;
prescaler_top = 0;
divider_top = 0;
timer_top = 0;
dt_top = 0;

for i = 1:9
    
    dt = prescaler(i) * t_instr;
    
    for divider = 1:600
        err = abs(t_des - timer * dt * divider);
        
        [val, index] = min(err);
        
        if abs(val) < err_min
            err_min = val;
            prescaler_top = prescaler(i);
            divider_top = divider;
            timer_top = timer(index);
            disp(err_min);
            dt_top = dt;
        end
    end
    
end

ppm = (1000000/t_des(1)) * err_min;
preload = dec2hex(hex2dec('FFFF') - timer_top + 1);


fprintf('prescaler:    %i \n', prescaler_top);
fprintf('divider:      %i \n', divider_top);
fprintf('preload:      0x%s \n', preload);
fprintf('error:        %0.4f ppm (~ %0.1f s in 4 months)\n',ppm, ppm*10);
fprintf('interval:     %0.4f ms', 1000* dt_top * timer_top);
fprintf('\n\n');