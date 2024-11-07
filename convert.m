for x=1:100
  A = delsq(numgrid('S', x + 2));
  [i,j,val] = find(A);
  data_dump = [i,j,val];
  fid = fopen(['convertedMats/' num2str(x) '.txt'],'w');
  fprintf( fid,'%d %d %f\n', transpose(data_dump) );
  fclose(fid);
end
