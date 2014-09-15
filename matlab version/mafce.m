function [IAVs,VTs] = mafce(BVs,IVs,VStep)
%MAFCE Triangular meshes via Advancing Front Method For Multi-boundary
    % function [IAVs,VTs] = mafce(BVs,IVs,VStep)
    % BVs is cells with m*2 matrix for boundary vertexes, and IVs is a n*2 matrix
    % for inner vertexes. VStep is the stantard step between vertexes. AFCE
    % return [vertexes coordination and index, triangles indexes sharing a vertex]
    IAVs=[IVs];
    for i=1:length(BVs)
        IAVs=[IAVs;BVs{i}];
    end
    IAVs=[IAVs,(1:size(IAVs,1))']; % indexed all vertexes(-1: nerver used; >0: usage count; 0: can't be used for out of boundary)
    flag=[-1.*ones(size(IVs,1),1);2.*ones(size(IAVs,1)-size(IVs,1),1)];
    IBVs=IAVs(size(IVs,1)+1:end,:);
    IAVs=sortrows(IAVs,[2,1]);
    FEs=cell(size(IBVs,1),2); % Front Edges
    m=1;
    n=1;
    for i=1:length(BVs)
        for j=1:size(BVs{i},1)-1
            FEs(m,:)={IBVs(n,:),IBVs(n+1,:)};
            m=m+1;
            n=n+1;
        end
        FEs(m,:)={IBVs(n,:),IBVs(n-size(BVs{i},1)+1,:)};
        m=m+1;
        n=n+1;
    end
    % show progress bar
    ptotal=size(IVs,1);
    pnow=0;
    hwait=waitbar(0,'Calculating...');
    i=1;
    while size(FEs,1)>3
        waitbar(pnow/ptotal,hwait,['Calculating...',num2str(floor(pnow/ptotal*100)),'%']);
%         for t=1:size(FEs,1)
%             plot([FEs{t,1}(1),FEs{t,2}(1)],[FEs{t,1}(2),FEs{t,2}(2)],'r-');
%         end % only for debug
%         pause(0.1); % only for debug
        tri={FEs{1,1},FEs{1,2},afmSearch(FEs{1,1},FEs{1,2},IAVs,FEs,VStep,flag)};
        VTs(i,:)=[tri{1}(3),tri{2}(3),tri{3}(3)];
%         plot([FEs{1,1}(1),FEs{1,2}(1)],[FEs{1,1}(2),FEs{1,2}(2)],'w-'); % only for debug
        if flag(tri{3}(	3))==-1
            pnow=pnow+1;
        end
        flag(FEs{1,1}(3))=flag(FEs{1,1}(3))-1;
        flag(FEs{1,2}(3))=flag(FEs{1,2}(3))-1;
        FEs(1,:)=[];
        j=1;
        found=[0,0];
        while j<=size(FEs,1)
            if (FEs{j,1}(3)==tri{1}(3)&&FEs{j,2}(3)==tri{3}(3))||(FEs{j,1}(3)==tri{3}(3)&&FEs{j,2}(3)==tri{1}(3))
%                 plot([FEs{j,1}(1),FEs{j,2}(1)],[FEs{j,1}(2),FEs{j,2}(2)],'w-'); % only for debug
                flag(FEs{j,1}(3))=flag(FEs{j,1}(3))-1;
                flag(FEs{j,2}(3))=flag(FEs{j,2}(3))-1;
                FEs(j,:)=[];
                found(1)=1;
                continue;
            end
            if (FEs{j,1}(3)==tri{3}(3)&&FEs{j,2}(3)==tri{2}(3))||(FEs{j,1}(3)==tri{2}(3)&&FEs{j,2}(3)==tri{3}(3))
%                 plot([FEs{j,1}(1),FEs{j,2}(1)],[FEs{j,1}(2),FEs{j,2}(2)],'w-'); % only for debug
                flag(FEs{j,1}(3))=flag(FEs{j,1}(3))-1;
                flag(FEs{j,2}(3))=flag(FEs{j,2}(3))-1;
                FEs(j,:)=[];
                found(2)=1;
                continue;
            end
            j=j+1;
        end
        if found(1)==0
            FEs(end+1,:)={tri{1},tri{3}};
            flag(FEs{end,1}(3))=flag(FEs{end,1}(3))+1;
            if flag(FEs{end,2}(3))==-1
                flag(FEs{end,2}(3))=1;
            else
                flag(FEs{end,2}(3))=flag(FEs{end,2}(3))+1;
            end
        end
        if found(2)==0
            FEs(end+1,:)={tri{3},tri{2}};
            flag(FEs{end,2}(3))=flag(FEs{end,2}(3))+1;
            if flag(FEs{end,1}(3))==-1
                flag(FEs{end,1}(3))=1;
            else
                flag(FEs{end,1}(3))=flag(FEs{end,1}(3))+1;
            end
        end
        i=i+1;
    end
    waitbar(1,hwait,'Finishing...');
    tri={FEs{1,1},FEs{1,2},FEs{2,2}};
    VTs(end+1,:)=[tri{1}(3),tri{2}(3),tri{3}(3)];
    close(hwait);
    IAVs=sortrows(IAVs,3);
end
