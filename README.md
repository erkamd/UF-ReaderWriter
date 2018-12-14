# Um-Reader
Basic UF (UmFormat) Reader/Writer that allows to get parameters and descriptions from .txt files with using a simple format.

Here are formats:
Description use: 
"<d* name *d> description *d>" If your text file includes this and you opened the text file with "openTxtFile" function, you will get "description" as a string result when you call "getDescription" function with "name" argument.

Parameter use: 
"<p* allParameters *p> par1, par2, par3, par4 *p>" If your text file includes this and you opened the text file with "openTxtFile" function, you will get the "par1", "par2", "par3", "par4" as a vector<string> result when you call "getParameters" function with "allParameters" argument.

