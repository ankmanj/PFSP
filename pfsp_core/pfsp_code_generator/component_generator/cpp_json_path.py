

import os 

class CPPJSONPath:
    def __init__(self):
        self.__dest_path = ''
        self.__source_path = ''
        #This is read access
        self.__access_rights = 0o755

    @property
    def source_path(self):
        return self.__source_path
    
    @source_path.setter
    def source_path(self, s):
        if not os.path.exists(s):
            print ("File {0} not found".format(s))
            exit(-1)
        self.__source_path = s
    
    
    @property
    def dest_path(self):
        return self.__dest_path
    
    @source_path.setter
    def dest_path(self, dest_path:str):
        if not os.path.exists(dest_path):
            try:
                self.__dest_path = dest_path
                os.mkdir(self.__dest_path, self.__access_rights)
            except OSError:
                print ("Creation of the directory %s failed" % self.__dest_path)
            else:
                print ("Successfully created the directory %s" % self.__dest_path)
        
        else:
            self.__dest_path = dest_path
            print("Directory {0} exists".format(self.__dest_path))


    def append_to_dest(self, filename:str):
        return os.path.join(self.__dest_path, filename)