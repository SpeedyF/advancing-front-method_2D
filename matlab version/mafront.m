% MAFRONT Advancing Front Method For Multi-boundary
    % Author: Guangmu Zhu
    % email: guangmuzhu@gmail.com
    % 2013/05/14

clc;
clear all;
close all;
%% ginput vertexes for polygon
MXY=input('width and height of polygon[x,y]: ');
figure(1);
hold on;
box on;
axis([0,MXY(1),0,MXY(2)]);

InMod=input('Plot polygon or File? P/F[P]: ','s');
if isempty(InMod)
    InMod='P';
end
InMod=upper(InMod);
VX=cell(0,0);
VY=cell(0,0);
if InMod=='P'
    fprintf('Right click is to close boundary, and middle click or any key to finish input.\nThe first boundary must be the outer boundary.\n');
    i=1;
    while true
        if length(VX)<i
            VX(i)={[]};
            VY(i)={[]};
        end
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
        if button==3
            VX{i}=[VX{i},VX{i}(1)];
            VY{i}=[VY{i},VY{i}(1)];
            plot(VX{i},VY{i},'r-');
            i=i+1;
            continue;
        end
        if button==2 || button>3
            if length(VX)<i
                VX{i}=[VX{i},VX{i}(1)];
                VY{i}=[VY{i},VY{i}(1)];
                plot(VX{i},VY{i},'r-');
            end
            VX(i)=[];
            VY(i)=[];
            break;
        end
        VX{i}=[VX{i},x];
        VY{i}=[VY{i},y];
        plot(VX{i},VY{i},'r-');
    end
elseif InMod=='F'
    file=input('File path: ','s'); % only .mat supported now.
    load(file);
    for i=1:length(VX)
        plot(VX{i},VY{i},'r-');
    end
else
    fprintf(2,'Error with input!');
    return;
end
%% check inner boundary
for i=2:length(VX)
    [IN,ON]=inpolygon(VX{i},VY{i},VX{1},VY{1});
    if sum(IN+ON)~=length(VX{i})
        fprintf(2,'Inner boundary out of outer boundary!\n');
        return;
    end
end
%% check direction
for i=1:length(VX)
    k1=0;
    k2=0;
    for j=1:length(VX{i})-2
        k2=mod(mod(cart2pol(VX{i}(j+2)-VX{i}(j+1),VY{i}(j+2)-VY{i}(j+1)),2*pi)-mod(cart2pol(VX{i}(j+1)-VX{i}(j),VY{i}(j+1)-VY{i}(j)),2*pi),2*pi);
        if k2>pi
            k2=k2-2*pi;
        end
        k1=k1+k2;
    end
    if i==1&&k1<0
        fprintf(2,'Outer boundary has negative direction, fixed.\n');
        VX{i}=fliplr(VX{i});
        VY{i}=fliplr(VY{i});
    elseif i>1&&k1>0
        fprintf(2,'Inner boundary has positive direction, fixed.\n');
        VX{i}=fliplr(VX{i});
        VY{i}=fliplr(VY{i});
    end
end
%% check intersecting lines
for m=1:length(VX)
    for i=1:length(VX{m})-1
        for n=m:length(VX)
            for j=1:1:length(VX{n})-1
                if m==n&&j<=i
                    continue;
                end
                if abs(det([VX{m}(i)-VX{m}(i+1),VX{n}(j+1)-VX{n}(j);VY{m}(i)-VY{m}(i+1),VY{n}(j+1)-VY{n}(j)]))<eps
                    continue;
                end
                k=[VX{m}(i)-VX{m}(i+1),VX{n}(j+1)-VX{n}(j);VY{m}(i)-VY{m}(i+1),VY{n}(j+1)-VY{n}(j)]\[VX{n}(j+1)-VX{m}(i+1);VY{n}(j+1)-VY{m}(i+1)];
                if (k(1)>1e-8&&k(1)<1-1e-8)&&(k(2)>1e-8&&k(2)<1-1e-8)
                    fprintf(2,'Intersecting lines found!\n');
                    return;
                end
            end
        end
    end
end
%% boundary interpolation
BMXY=[min(VX{1}),max(VX{1});min(VY{1}),max(VY{1})];
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
% BVs: Boundary Vertexes
for i=1:length(VX)
    BVs(i)={[]};
    for j=1:length(VX{i})-1
        BVs{i}=[BVs{i};VX{i}(j),VY{i}(j)];
        k=sqrt((VX{i}(j)-VX{i}(j+1))^2+(VY{i}(j)-VY{i}(j+1))^2)/VStep;
        if k>=4.5 % 10% error of VStep between vertexes is allowed
            k=round(k);
            tmp1=linspace(VX{i}(j),VX{i}(j+1),k+1);
            tmp2=linspace(VY{i}(j),VY{i}(j+1),k+1);
            BVs{i}=[BVs{i};[tmp1(2:end-1);tmp2(2:end-1)]'];
        elseif k>=1.8 % 90% VStep between vertexes is allowed
            if k<2.7
                BVs{i}=[BVs{i};(VX{i}(j)+VX{i}(j+1))/2,(VY{i}(j)+VY{i}(j+1))/2];
            elseif k<3.6
                tmp1=linspace(VX{i}(j),VX{i}(j+1),4);
                tmp2=linspace(VY{i}(j),VY{i}(j+1),4);
                BVs{i}=[BVs{i};[tmp1(2:end-1);tmp2(2:end-1)]'];
            else
                tmp1=linspace(VX{i}(j),VX{i}(j+1),5);
                tmp2=linspace(VY{i}(j),VY{i}(j+1),5);
                BVs{i}=[BVs{i};[tmp1(2:end-1);tmp2(2:end-1)]'];
            end
        else
            continue;
        end
    end
end
for i=1:length(BVs)
    plot(BVs{i}(:,1),BVs{i}(:,2),'k.');
end
%% inner interpolation
IBVs=[0,0,0]; % Indexed Boundary Vertexes for inner interpolation:[Y index of LYs, X, Y]
for i=2:length(LYs)-1
    for m=1:length(VX)
        for j=1:length(VX{m})-1
            k=(LYs(i)-VY{m}(j))/(VY{m}(j+1)-VY{m}(j));
            if k>1e-8&&k<1-1e-8
                IBVs=[IBVs;i,(VX{m}(j+1)-VX{m}(j))*k+VX{m}(j),LYs(i)];
            elseif k>-1e-8&&k<1e-8 % k==0 should be taken into consideration
                if j==1
                    if (VY{m}(2)-VY{m}(1))*(VY{m}(1)-VY{m}(end-1))>0
                        IBVs=[IBVs;i,(VX{m}(j+1)-VX{m}(j))*k+VX{m}(j),LYs(i)];
                    end
                else
                    if (VY{m}(j+1)-VY{m}(j))*(VY{m}(j)-VY{m}(j-1))>0
                        IBVs=[IBVs;i,(VX{m}(j+1)-VX{m}(j))*k+VX{m}(j),LYs(i)];
                    end
                end
            end
        end
    end
end
IBVs(1,:)=[];
IBVs=sortrows(IBVs,[1,2]);
IVs=[0,0]; % Inner Vertexex
for i=1:2:size(IBVs,1)-1
    k=(IBVs(i+1,2)-IBVs(i,2))/VStep;
    if k>=4.5 % 10% error of VStep between vertexes is allowed
        k=round(k);
        tmp=linspace(IBVs(i,2),IBVs(i+1,2),k+1);
        IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,k-1)]'];
    elseif k>=1.8 % 90% VStep between vertexes is allowed
        if k<2.7
            IVs=[IVs;(IBVs(i,2)+IBVs(i+1,2))/2,LYs(IBVs(i,1))];
        elseif k<3.6
            tmp=linspace(IBVs(i,2),IBVs(i+1,2),4);
            IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,2)]'];
        else
            tmp=linspace(IBVs(i,2),IBVs(i+1,2),5);
            IVs=[IVs;[tmp(2:end-1);repmat(LYs(IBVs(i,1)),1,3)]'];
        end
    else
        continue;
    end
end
IVs(1,:)=[];
i=1;
while i<=size(IVs,1)
    label=false;
    for m=1:length(BVs)
        for j=1:length(BVs{m})
            if sqrt((IVs(i,1)-BVs{m}(j,1))^2+(IVs(i,2)-BVs{m}(j,2))^2)<0.9*VStep
                IVs(i,:)=[];
                label=true;
                i=i-1;
                break;
            end
        end
        if label
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
[IAVs,ITs]=mafce(BVs,IVs,VStep);
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