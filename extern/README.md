# PSE 
Pitch Spelling algorithms

external modules
added with `git subtree`

 
- **spdlog**  (gabime)  
  https://github.com/gabime/spdlog.git  
  branch: v1.x  
  see https://github.com/pybind/pybind11  
  doc https://pybind11.readthedocs.io/en/latest/    
 
- **pybind11**  
  https://github.com/pybind/pybind11.git  
  branch: master  
  see https://github.com/gabime/spdlog   
  
- **googletest**
  https://github.com/google/googletest.git
  branch: main
  

update code with  
```shell
git subtree pull --prefix extern/spdlog spdlog v1.x --squash
git subtree pull --prefix extern/pybind11 pybind11 master --squash
git subtree pull --prefix extern/googletest googletest main --squash
```

`spdlog`  `pybind11` `googletest` are short names of remote git repositories, 
see
```shell
git remote -v
git remote add spdlog https://github.com/gabime/spdlog.git
git remote add pybind11 https://github.com/pybind/pybind11.git 
git remote add googletest https://github.com/google/googletest.git
```


