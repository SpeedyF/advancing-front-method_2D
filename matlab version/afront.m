% AFRONT Advancing Front Method
    % Author: Guangmu Zhu
    % email: guangmuzhu@gmail.com
clc;
clear all;
close all;
%% ginput vertexes for polygon
MXY=input('width and height of polygon[x,y]: ');
figure(1);
hold on;
box on;
axis([0,MXY(1),0,MXY(2)]);

VX=zeros(1,1);
VY=zeros(1,1);

InMod=input('Curve or Polygon or File? C/P/F[P]: ','s');
if isempty(InMod)
    InMod='P';
end
InMod=upper(InMod);
if InMod=='P'
    i=1;
    while true
        [x,y,button]=ginput(1);
        if x<0||x>MXY(1)||y<0||y>MXY(2)
            continue
        end
        %{
        [x,y,button] = ginput(...)
            return coodinates of x and y
            as well as code of button
            (1=left, 2=mid, 3=right, otherwise ASCII code)
        %}
        if button~=1
            break;
        end
        if i>1&&(x==VX(1)&&y==VY(1))
            break;
        end
        VX(i)=x;
        VY(i)=y;
        i=i+1;
        plot(VX,VY,'r-');
    end
elseif InMod=='C'
    VStep=input('Step between vertexes is: ');
%     set(gcf,'WindowButtonDownFcn',@LButtonDownFcn);
%     set(gcf,'WindowButtonMotionFcn',@LButtonMotionFcn);
%     set(gcf,'WindowButtonUpFcn',@LButtonUpFcn);
    set(gcf,'WindowButtonDownFcn','LButtonFcn(''down'',VStep)');
    set(gcf,'WindowButtonMotionFcn','LButtonFcn(''move'',VStep)');
    set(gcf,'WindowButtonUpFcn','LButtonFcn(''up'',VStep)');
    disp('Curve finished? Any key to continue...');
    pause;
    load('Vs.mat');
elseif InMod=='F'
    file=input('File path: ','s');
    Vs=load(file);
    VX=Vs(1,:);
    VY=Vs(2,:);
else
    fprintf(2,'Error with input!');
    return;
end
if size(VX,2)>2
    VX=[VX,VX(1)];
    VY=[VY,VY(1)];
    plot(VX,VY,'r-');
    plot(VX,VY,'b.');
else
    fprintf(2,'More vertexes required!\n');
    return;
end
%% turned to the positive direction
k1=0;
k2=0;
for i=1:size(VX,2)-2
    k2=mod(mod(cart2pol(VX(i+2)-VX(i+1),VY(i+2)-VY(i+1)),2*pi)-mod(cart2pol(VX(i+1)-VX(i),VY(i+1)-VY(i)),2*pi),2*pi);
    if k2>pi
        k2=k2-2*pi;
    end
    k1=k1+k2;
end
if k1<0
    fprintf(2,'negative direction, fixed.\n');
    VX=fliplr(VX);
    VY=fliplr(VY);
end
%% check intersecting lines
% if size(VX,2)~=3
%     for i=4:size(VX,2)-1
%         [IN,ON]=inpolygon(VX(i),VY(i),VX(1:i-1),VY(1:i-1));
%         if ON
%             fprintf(2,'Vertex is on lines!\n');
%             return;
%         end
%         if ~IN
%             % concave polygon?
%             [IN,ON]=inpolygon(VX(i-1),VY(i-1),VX(1:i-2),VY(1:i-2));
%             if IN
%                 prefix=-1;
%             else
%                 prefix=1;
%             end
%             pol1=mod(cart2pol(VX(i-2)-VX(i-1),VY(i-2)-VY(i-1)),2*pi);
%             pol2=mod(cart2pol(VX(1)-VX(i-1),VY(1)-VY(i-1)),2*pi);
%             pol3=mod(cart2pol(VX(i)-VX(i-1),VY(i)-VY(i-1)),2*pi);
%             if prefix*(pol3(1)-pol1(1))*(pol3(1)-pol2(1))<0
%                 fprintf(2,'Intersecting lines found!\n');
%                 return;
%             end
%         end
%     end
% end

if size(VX,2)~=3
    for i=1:size(VX,2)-3
        for j=i+2:size(VX,2)-1
            if i==1&&j==size(VX,2)-1
                continue;
            end
            if abs(det([VX(i)-VX(i+1),VX(j+1)-VX(j);VY(i)-VY(i+1),VY(j+1)-VY(j)]))<eps
                continue;
            end
            k=[VX(i)-VX(i+1),VX(j+1)-VX(j);VY(i)-VY(i+1),VY(j+1)-VY(j)]\[VX(j+1)-VX(i+1);VY(j+1)-VY(i+1)];
            if (k(1)>1e-8&&k(1)<1-1e-8)&&(k(2)>1e-8&&k(2)<1-1e-8)
                fprintf(2,'Intersecting lines found!\n');
                return;
            end
        end
    end
end
%% boundary interpolation
BMXY=[min(VX),max(VX);min(VY),max(VY)];
while true
    VStep=input('Grid size: ');
    k=(BMXY(2,2)-BMXY(2,1))/VStep;
    if k>=4.5 % 10% error of VStep between vertexes is allowed
        k=round(k);
        break;
    else
        fprintf(2,'Size is too large.\n');
    end
end
LYs=BMXY(2,1):(BMXY(2,2)-BMXY(2,1))/k:BMXY(2,2);
BVs=[0,0]; % Boundary Vertexes
for i=1:size(VX,2)-1
    BVs=[BVs;VX(i),VY(i)];
    k=sqrt((VX(i)-VX(i+1))^2+(VY(i)-VY(i+1))^2)/VStep;
    if k>=4.5 % 10% error of VStep between vertexes is allowed
        k=round(k);
        tmp1=linspace(VX(i),VX(i+1),k+1);
        tmp2=linspace(VY(i),VY(i+1),k+1);
        BVs=[BVs;[tmp1(2:end-1);tmp2(2:end-1)]'];
    elseif k>=1.8 % 90% VStep between vertexes is allowed
        if k<2.7
            BVs=[BVs;(VX(i)+VX(i+1))/2,(VY(i)+VY(i+1))/2];
        elseif k<3.6
            tmp1=linspace(VX(i),VX(i+1),4);
            tmp2=linspace(VY(i),VY(i+1),4);
            BVs=[BVs;[tmp1(2:end-1);tmp2(2:end-1)]'];
        else
            tmp1=linspace(VX(i),VX(i+1),5);
            tmp2=linspace(VY(i),VY(i+1),5);
            BVs=[BVs;[tmp1(2:end-1);tmp2(2:end-1)]'];
        end
    else
        continue;
    end
end
BVs(1,:)=[];
plot(BVs(:,1),BVs(:,2),'k.');
%% inner interpolation
IBVs=[0,0,0,0]; % Indexed Boundary Vertexes for inner interpolation:[Y index, Edge vertex index, X, Y]
for i=2:size(LYs,2)-1
    for j=1:size(VX,2)-1
        k=(LYs(i)-VY(j))/(VY(j+1)-VY(j));
        if k>1e-8&&k<1-1e-8
            IBVs=[IBVs;i,j,(VX(j+1)-VX(j))*k+VX(j),LYs(i)];
        elseif k>-1e-8&&k<1e-8 % k==0 should be taken into consideration
            if j==1
                if (VY(2)-VY(1))*(VY(1)-VY(end-1))>0
                    IBVs=[IBVs;i,(VX(j+1)-VX(j))*k+VX(j),LYs(i)];
                end
            else
                if (VY(j+1)-VY(j))*(VY(j)-VY(j-1))>0
                    IBVs=[IBVs;i,(VX(j+1)-VX(j))*k+VX(j),LYs(i)];
                end
            end
        end
    end
end
IBVs(1,:)=[];
IBVs=sortrows(IBVs,[1,3]);
IVs=[0,0]; % Inner Vertexex
for i=1:2:size(IBVs,1)-1
    k=(IBVs(i+1,3)-IBVs(i,3))/VStep;
    if k>=4.5 % 10% error of VStep between vertexes is allowed
        k=round(k);
        tmp=linspace(IBVs(i,3),IBVs(i+1,3),k+1);
        IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,k-1)]'];
    elseif k>=1.8 % 90% VStep between vertexes is allowed
        if k<2.7
            IVs=[IVs;(IBVs(i,3)+IBVs(i+1,3))/2,LYs(IBVs(i,1))];
        elseif k<3.6
            tmp=linspace(IBVs(i,3),IBVs(i+1,3),4);
            IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,2)]'];
        else
            tmp=linspace(IBVs(i,3),IBVs(i+1,3),5);
            IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,3)]'];
        end
    else
        continue;
    end
end
IVs(1,:)=[];
i=1;
while i<=size(IVs,1)
    for j=1:size(BVs,1)
        if sqrt((IVs(i,1)-BVs(j,1))^2+(IVs(i,2)-BVs(j,2))^2)<0.9*VStep
            IVs(i,:)=[];
            i=i-1;
            break;
        end
    end
    i=i+1;
end
IVs=sortrows(IVs,[2,1]);
plot(IVs(:,1),IVs(:,2),'k.');
%% create elements
% IAVs: [ x, y, node index]
% ITs: [ node index, node index, node index ]
[IAVs,ITs]=afce(BVs,IVs,VStep);
for i=1:size(ITs,1)
    plot([IAVs(ITs(i,1),1),IAVs(ITs(i,2),1),IAVs(ITs(i,3),1),IAVs(ITs(i,1),1)],[IAVs(ITs(i,1),2),IAVs(ITs(i,2),2),IAVs(ITs(i,3),2),IAVs(ITs(i,1),2)],'r-');
end
%% smooth triangles
figure(2);
hold on;
box on;
axis([0,MXY(1),0,MXY(2)]);
rf=[];
if isempty(rf)||rf<0
    rf=input('Relaxation factor:(>0) ');
end
[SIAVs,err]=triSmooth(IAVs,size(IVs,1),ITs,rf,VStep,1e-4,1e-2);
fprintf('%.2f%% Error\n',err*100);
for i=1:size(ITs,1)
    plot([SIAVs(ITs(i,1),1),SIAVs(ITs(i,2),1),SIAVs(ITs(i,3),1),SIAVs(ITs(i,1),1)],[SIAVs(ITs(i,1),2),SIAVs(ITs(i,2),2),SIAVs(ITs(i,3),2),SIAVs(ITs(i,1),2)],'r-');
end