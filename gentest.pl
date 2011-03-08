#!/usr/bin/perl

for (my $i = 0 ; $i < $ARGV[0] ; $i++) {
    printf("%d %d\n", rand()*255, rand()*255); 
}
