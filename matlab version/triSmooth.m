function [IAVs,err] = triSmooth(IAVs,count,ITs,rf,VStep,CC,EC)
%TRISMOOTH Smooth triangles via making vertexes central
    % function [IAVs,err] = triSmooth(IAVs,count,ITs,rf,VStep,CC,EC)
    % IAVs: [ x, y, node index ], [ inner vertexes; boundary vertexes ]
    % count: inner vertexes count
    % ITs: indexed triangles
    % rf: relaxation factor, >0
    % VStep: standard step between vertexes.
    % CC: convergence coefficient
    % EC: error coefficient
    VVs=cell(size(IAVs,1),1);
    for i=1:size(ITs,1)
        VVs(ITs(i,1))={[VVs{ITs(i,1)},ITs(i,2),ITs(i,3)]};
        VVs(ITs(i,2))={[VVs{ITs(i,2)},ITs(i,3),ITs(i,1)]};
        VVs(ITs(i,3))={[VVs{ITs(i,3)},ITs(i,1),ITs(i,2)]};
    end
    VVs=VVs(1:count);
    for i=1:size(VVs,1)
        VVs(i)={unique(VVs{i})};
    end
    err1=2*VStep;
    err2=VStep;
    while err1-err2>=VStep*CC||err2>VStep*EC
        err1=err2;
        err2=0;
        for i=1:size(VVs,1)
            tmp=VVs{i};
            tmp1=0;
            tmp2=0;
            for j=1:length(tmp)
                tmp1=tmp1+IAVs(tmp(j),1)-IAVs(i,1);
                tmp2=tmp2+IAVs(tmp(j),2)-IAVs(i,2);
            end
            err2=max(err2,sqrt(tmp1^2+tmp2^2));
            IAVs(i,1)=IAVs(i,1)+rf/length(tmp)*tmp1;
            IAVs(i,2)=IAVs(i,2)+rf/length(tmp)*tmp2;
        end
    end
    err=err2/VStep;
end