import os
import sys
import numpy as np
from math import floor as floor

cols = int(os.popen('tput cols').read()[:-1])
rows = int(os.popen('tput lines').read()[:-1])
dash = cols * '-' 
div  = cols * '#'

def print_title(title, char='#'):
    title = ' '+title+' '
    hlen_title = floor(len(title)/2)
    hlen_cols  = floor(cols/2)
    divider = int(hlen_cols - hlen_title)
    rest = cols - (divider + len(title))
    print(divider*char+title+rest*char)
    return True
    
def create_alias(aliases_src_file, alias_name, content):
    content = content.replace("$", "\$")
    content = os.path.expandvars(content)
    alias_file_exist    = os.path.isfile(aliases_src_file)
    get_aliases     = ''
    if ( alias_file_exist ):
        get_aliases     = os.popen("cat "+aliases_src_file).read()[:-1]

    if ( "alias "+alias_name not in get_aliases ):
        print(">>> Setting alias "+alias_name)
        os.system("echo \"alias "+alias_name+"=\'"+content+"\'\" >> "+aliases_src_file)
    else:
        tmp = np.array(get_aliases.split("\n"))
        idx = -1
        for i in range ( tmp.size ):
            if ( "alias "+alias_name in tmp[i] ):
                idx = i
                
        tmp_content = tmp[idx][tmp[idx].find('=')+1:].replace("'", "")
        if ( tmp_content != content ):
            print(">>> Update alias "+alias_name)
            tmp[idx] = alias_name+"=\'"+content+"\'"
            os.system("cat "+aliases_src_file+" > "+aliases_src_file+".tmp")
            os.system("grep -vwE \""+tmp_content+"\" "+aliases_src_file+".tmp > "+aliases_src_file+"")
            os.system("echo \"alias "+alias_name+"=\'"+content+"\'\" >> "+aliases_src_file)
            os.system("rm "+aliases_src_file+".tmp")
            os.system(alias_name+"=\'"+content+"\'")
    return True

def create_variable(envar_src_file, envar_name, content):
    envar_file_exist    = os.path.isfile(envar_src_file)
    get_envar           = ''
    if ( envar_file_exist ):
        get_envar       = os.popen("cat "+envar_src_file).read()[:-1]
    
    if ( envar_name not in get_envar ):
        print(">>> Setting envar "+envar_name)
        os.system("echo \"export "+envar_name+"="+content+"\" >> "+envar_src_file)
    else:  
        tmp = np.array(get_envar.split("\n"))
        for i in range ( tmp.size ):
            if ( envar_name in tmp[i] and content != tmp[i].split("=")[1] ):
                print(">>> Update envar "+envar_name)
                os.system("cat "+envar_src_file+" > "+envar_src_file+".tmp")
                os.system("grep -vwE \""+tmp[i].split("=")[1]+"\" "+envar_src_file+".tmp > "+envar_src_file+"")
                os.system("echo \"export "+envar_name+"="+content+" >> "+envar_src_file)
                os.system("rm "+envar_src_file+".tmp")
    os.system("export "+envar_name+"=\""+content+"\"")
    return True

################# SETTING UP GIT REPOSITORY #################
os.system("clear")
print_title("Setting up Git")
repo_dir_name   = "hexapod_project"
repo            = "https://github.com/colayare/hexapod_project.git"
#### 
git_dir         = "/home/root/"+repo_dir_name
gitdir_exists   = os.path.exists(git_dir)
git_exists      = False

if ( "-no_git" not in sys.argv ):

    if ( not gitdir_exists ):
        print(">>> Creating Git foler : "+git_dir)
        os.system("mkdir "+git_dir)
    else:
        git_exists = ".git" in os.popen("ls "+git_dir+" -1a").read()[:-1].split("\n")

    os.chdir(git_dir)

    if ( not git_exists ):
        print(">>> Setting up Git repository...")
        gitUserName = raw_input("\tEnter Git user name: ")
        gitUserMail = raw_input("\tEnter Git user email:")
        os.system("git init")
        os.system("git remote add -f origin "+repo)
        os.system("git config core.sparseCheckout true")
        os.system("git config branch.autosetuprebase always")
        os.system("git config --global user.email \""+gitUserMail+"\"")
        os.system("git config --global user.name \""+gitUserName+"\"")
        os.system("git config --global http.sslverify false")
        os.system("echo \"Modeling\" >> .git/info/sparse-checkout")
        
    if ( ".git" in os.popen("ls "+git_dir+" -1a").read()[:-1].split("\n") ):
        print(">>> Pulling from remote repository...")
        os.system("git pull --depth=1 origin master")
        
################# SETTING UP ENVIRONMENT VARS #################
print_title("Exporting environment variables")
envar_src_file      = "/home/root/.profile"
profile_exists      = os.path.isfile(envar_src_file)
if ( profile_exists ):
    get_profile     = os.popen("cat "+envar_src_file).read()[:-1]
else:
    get_profile     = ''

create_variable(envar_src_file, "PROJ_PATH", git_dir)
create_variable(envar_src_file, "MODEL_PY_PATH", git_dir+"/Modeling/Python")
create_variable(envar_src_file, "MODEL_C_PATH", git_dir+"/Modeling/C")
create_variable(envar_src_file, "MODEL_CPP_PATH", git_dir+"/Modeling/CPP")
create_variable(envar_src_file, "STARTUP_SCRIPT", "/etc/init.d/mystartup")

################# SETTING UP ALIASES #################
print_title("Setting up aliases")
aliases_src_file    = "/home/root/.bashrc"
alias_exist         = os.path.isfile(aliases_src_file)
if ( alias_exist ):
    get_aliases     = os.popen("cat "+aliases_src_file).read()[:-1]
else:
    get_aliases     = ''

create_alias(aliases_src_file, "cdpy", "cd $MODEL_PY_PATH")
create_alias(aliases_src_file, "cdc", "cd $MODEL_C_PATH")
create_alias(aliases_src_file, "cdcpp", "cd $MODEL_CPP_PATH")
create_alias(aliases_src_file, "REPO_PULL", "git pull --depth=1 origin master --rebase --autostash")
create_alias(aliases_src_file, "py", "python")
create_alias(aliases_src_file, "SET_OFFSETS", "python $MODEL_PY_PATH/set_hexapod_offsets.py")
create_alias(aliases_src_file, "TEST_GAIT", "python $MODEL_PY_PATH/test_gait.py")
create_alias(aliases_src_file, "SET_INIT", "python $MODEL_PY_PATH/set_initial_position.py")
create_alias(aliases_src_file, "TEST_LOCOMOTION", "python $MODEL_PY_PATH/test_locomotion.py")
create_alias(aliases_src_file, "TEST_TRANSLATION", "python $MODEL_PY_PATH/test_locomotion_translation.py")

get_profile     = os.popen("cat "+envar_src_file).read()[:-1]
if ( "source "+aliases_src_file not in get_profile ):
    os.system("echo \"source "+aliases_src_file+"\" >> "+envar_src_file)
    
################# SETTING UP STARTUP CONFIGURATION #################
print_title("Setting up startup configuration")
startup_file = "mystartup"
startup_exists  = os.path.isfile("/etc/init.d/"+startup_file)

if( not startup_exists ):
    print(">> Creating startup script file : /etc/init.d/"+startup_file)
    os.system("echo \"#!/bin/bash\necho \"Custom startup script\"\" > /etc/init.d/"+startup_file)
    os.system("chmod 777 /etc/init.d/"+startup_file)

if ( not os.path.isfile("/etc/rc5.d/S99"+startup_file) ):
    print(">> Create link to startup script file")
    os.system("ln -s /etc/init.d/"+startup_file+" /etc/rc5.d/S99"+startup_file)
    
os.system("rm /home/root/*~")
