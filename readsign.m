function sig = readsign(fid)

% read the number of active bins in this signature
num_active = fread(fid,1,'integer*8=>double');

sig = sparse(num_active,1);

for i = 1:num_active
    % read the active address, convert zero base to one base
    addr = fread(fid,1,'integer*8=>double') + 1;

    % read the activity level
    s = fread(fid,1,'uint8=>double');

    sig(addr) = s;

    fprintf('\r%06d / %06d', i, addr);
    fflush(stdout);
end

fprintf('\n');

