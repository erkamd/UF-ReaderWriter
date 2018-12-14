# Um-Reader
Basic UF (UmFormat) Reader that allows to get parameters and descriptions from .txt files with using a simple format.

Here are formats:
Description use: 
"<d* name *d> description *d>" If your text file includes this and you opened the text file with "openTxtFile" function, you will get "description" as a string result when you call "getDescription" function with "name" argument.

Parameter use: 
"<p* allParameters *p> par1, par2, par3, par4 *p>" If your text file includes this and you opened the text file with "openTxtFile" function, you will get the "par1", "par2", "par3", "par4" as a vector<string> result when you call "getParameters" function with "allParameters" argument. You can expand or narrow the list. There is not limit.

Please be careful about spaces between format characters and your custom text.

Some examples from "The Nature" PC Game:

<d* Language *d> auto *d>
<d* Music Level *d> 0 *d>
<d* SFX Level *d> 3 *d>

                   radius,  maxHP,  maxsta, str, int, agi, vit, 
<p* bat *p>          32.0,  300.0,   220.0,  20,   5,   8,  25 *p>
<p* bear *p>         32.0,  580.0,   120.0,  30,   6,  10,  35 *p>


