function LButtonFcn(mode,step)
    persistent VX;
    persistent VY;
    persistent i;
    persistent LeftDown;
    if isempty(i)
        i=1;
    end
    if isempty(LeftDown)
        LeftDown=false;
    end
    if strcmp(get(gcf,'SelectionType'),'normal')
    % strcmp(get(gcf,'SelectionType'),'alt')
        switch(mode)
            case 'down'
                LeftDown=true;
                i=1;
                pt=get(gca,'CurrentPoint');
                VX(i)=pt(1,1);
                VY(i)=pt(1,2);
                i=i+1;
            case 'move'
                if LeftDown==true
                    pt=get(gca,'CurrentPoint');
                    if sqrt((pt(1,1)-VX(i-1))^2+(pt(1,2)-VY(i-1))^2)>step
                        VX(i)=pt(1,1);
                        VY(i)=pt(1,2);
                        i=i+1;
                        plot(VX,VY,'r-');
                    end
                end
            case 'up'
                LeftDown=false;
                set(gcf,'WindowButtonDownFcn','');
                set(gcf,'WindowButtonMotionFcn','');
                set(gcf,'WindowButtonUpFcn','');
                save('Vs.mat','VX','VY');
        end
    end
end

