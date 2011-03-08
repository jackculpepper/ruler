function writesign(fid, sign)

idx = find(sign);

% write the number of active bins in this signature
fwrite(fid,length(idx),'integer*8');

%length(idx)

for i = 1:length(idx)
    % write the active address
    % convert matlab array numbering (1 based) to c numbering (0 based)
    fwrite(fid,idx(i)-1,'integer*8');

%    idx(i)

    % write the activity level
    fwrite(fid,sign(idx(i)),'uint8');
    
%    sign(idx(i))
    
end

