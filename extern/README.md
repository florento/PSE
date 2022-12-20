# PSE 
Pitch Spelling algorithms

external modules
added with `git subtree`

 
- spdlog   gabime
  https://github.com/gabime/spdlog.git
  branch: v1.x
  see   
        - https://github.com/pybind/pybind11
        - https://pybind11.readthedocs.io/en/latest/    
 
- pybind11 
  https://github.com/pybind/pybind11.git
  branch: master
  see
        - https://github.com/gabime/spdlog    
  

update code with

```shell
git subtree pull --prefix extern/spdlog spdlog v1.x --squash
git subtree pull --prefix extern/pybind11 pybind11 master --squash
```

`spdlog` and `pybind11` are shorter names of remote git repositories, see
```shell
git remote -v
```


