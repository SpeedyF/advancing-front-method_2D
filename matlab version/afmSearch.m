function innerNearest = afmSearch(pt1,pt2,Vs,FEs,VStep,flag)
% AFMSEARCH Search the nearest point from pt1 and pt2 in Vs on the left
% side
    % function innerNearest = afmSearch(pt1,pt2,Vs,VStep,flag,except)
    % vector is from pt1 to pt2, Vs is a n*2 matrix with [x,y] and Vs
    % must be sorted by sortrows(Vs,[2,1]).FEs is a collection of cells for
    % front egdes. innerNearest is the nearest point from pt1 and pt2
    % according to sum of square. VStep is the standard step between
    % vertexes. flag: see afce.m
    MXY=[min(pt1(1),pt2(1))-2*VStep,max(pt1(1),pt2(1))+2*VStep;min(pt1(2),pt2(2))-2*VStep,max(pt1(2),pt2(2))+2*VStep];
    for i=1:size(Vs,1)
        if Vs(i,2)>=MXY(2,1)
            break;
        end
    end
    minDist=2*(3*VStep)^2; % actually it can't be larger than this.
    while Vs(i,2)<=MXY(2,2)
        if Vs(i,1)>=MXY(1,1)&&Vs(i,1)<=MXY(1,2)&&flag(Vs(i,3))~=0&&~segCrs(pt1,pt2,Vs(i,1:2),FEs)&&isLeftSide(pt1,pt2,Vs(i,:))
            currDist=(Vs(i,1)-pt1(1))^2+(Vs(i,2)-pt1(2))^2+(Vs(i,1)-pt2(1))^2+(Vs(i,2)-pt2(2))^2;
            if minDist>=currDist
                minDist=currDist;
                innerNearest=i;
            end
        end
        i=i+1;
        if i>size(Vs,1)
            break;
        end
    end
    innerNearest=Vs(innerNearest,:);
end

function FOUND=segCrs(pt1,pt2,pt3,FEs)
    for i=1:size(FEs,1)
        if abs(det([pt1(1)-pt3(1),FEs{i,2}(1)-FEs{i,1}(1);pt1(2)-pt3(2),FEs{i,2}(2)-FEs{i,1}(2)]))<eps
            continue;
        end
        k=[pt1(1)-pt3(1),FEs{i,2}(1)-FEs{i,1}(1);pt1(2)-pt3(2),FEs{i,2}(2)-FEs{i,1}(2)]\[FEs{i,2}(1)-pt3(1);FEs{i,2}(2)-pt3(2)];
        if (k(1)>1e-8&&k(1)<1-1e-8)&&(k(2)>1e-8&&k(2)<1-1e-8)
            FOUND=true;
            return;
        end
        if abs(det([pt2(1)-pt3(1),FEs{i,2}(1)-FEs{i,1}(1);pt2(2)-pt3(2),FEs{i,2}(2)-FEs{i,1}(2)]))<eps
            continue;
        end
        k=[pt2(1)-pt3(1),FEs{i,2}(1)-FEs{i,1}(1);pt2(2)-pt3(2),FEs{i,2}(2)-FEs{i,1}(2)]\[FEs{i,2}(1)-pt3(1);FEs{i,2}(2)-pt3(2)];
        if (k(1)>1e-8&&k(1)<1-1e-8)&&(k(2)>1e-8&&k(2)<1-1e-8)
            FOUND=true;
            return;
        end
    end
    FOUND=false;
end
