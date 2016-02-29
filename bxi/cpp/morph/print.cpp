logfile << kol_hanituxim.count() << endl; 
LOOPVECTOR(;,kol_hanituxim,i)  {
  logfile << kol_hanituxim.sikui(i) << " ";
  kol_hanituxim.info(i).ktov_cafuf(logfile);
  logfile <<" ";     
  kol_hanituxim.info(i).ktov_barur (logfile, hamila);
  logfile << " (";
  kol_hanituxim.info(i).ktov_meforat(logfile);
  logfile << ")" <<endl;
}
