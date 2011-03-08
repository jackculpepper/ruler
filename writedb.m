
fid = fopen('signdb.bin','wb');

% write the number of signatures in the db
fwrite(fid,length(database),'integer*8');

%length(database)

for k = 1:length(database)

    sign = database(k).sign;

    writesign(fid, sign);

    fprintf('\r%d / %d', k, length(database));
    fflush(stdout);
end
fprintf('\n');

fclose(fid);


