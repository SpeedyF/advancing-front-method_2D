function ls = isLeftSide(pt1,pt2,pt3)
% isLeftSide: is pt3 on the left side of vector from pt1 to pt2?
    % function ls = isLeftSide(pt1,pt2,pt3)
    if (pt3(1)==pt1(1)&&pt3(2)==pt1(2))||(pt3(1)==pt2(1)&&pt3(2)==pt2(2))
        ls=false;
        return;
    end
% Lack of precision
%     if mod(mod(cart2pol(pt2(1)-pt1(1),pt2(2)-pt1(2)),2*pi)-mod(cart2pol(pt3(1)-pt1(1),pt3(2)-pt1(2)),2*pi),2*pi)>pi
    if (pt1(2)-pt2(2))*pt3(1)-(pt1(1)-pt2(1))*pt3(2)+(pt1(1)*pt2(2)-pt2(1)*pt1(2))>1e-8 % this number should be reconsidered
        ls=true;
    else
        ls=false;
    end
end

