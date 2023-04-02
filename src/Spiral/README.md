# Elaine Chew's spiral of fifths
pitch-spelling without prior tonality estimation

## Definition Spiral
see `spiral.pdf`

index of note names in the line of fifths relative to `C`, 
values between -15 (`Fbb`) and 19 (`B##`)
- `C` has index 0 
- index increases in the direction of sharps:
  `G` has index 1, `D`  has index 2, `F#` has index 6...
- `F` has index -1, `Bb` has index -2...


## Principle
Every input MIDI pitch is associated a choosen note name + accid.  
A choosen name corresponds to a point in the piral.

- Compute a barycenter for a window of last chosen names 
  (= points in spiral)
- for a MIDI pitch, choose the possible name the closet, in spiral, 
  to the barycenter.

## Refs

> Elaine Chew and Yun-Ching Chen  
  Real-Time Pitch Spelling Using the Spiral Array  
  Computer Music Journal, 29(2), 61–76 (2005)
https://www.mitpressjournals.org/doi/abs/10.1162/0148926054094378

> Chew, E., Chen, Y.-C.  
  Determining context-defining windows: pitch spelling using the Spiral Array.  
  in Proceedings of the 4th International Conference for Music Information Retrieval, pp. 26–30 (ISMIR 2003)
  http://ismir2003.ismir.net/posters/Chew.pdf  
[Google Scholar](https://scholar.google.com/scholar?q=Chew%2C%20E.%2C%20Chen%2C%20Y.-C.%3A%20Determining%20context-defining%20windows%3A%20pitch%20spelling%20using%20the%20Spiral%20Array.%20In%3A%20Proceedings%20of%20the%204th%20International%20Conference%20for%20Music%20Information%20Retrieval%2C%20pp.%2026%E2%80%9330%20%282003%29)

> Chew, E.  
> Towards a mathematical model of tonality.  
> Ph.D. thesis, Massachusetts Institute of Technology, Cambridge, MA (2000)
[Google Scholar](https://scholar.google.com/scholar?q=Chew%2C%20E.%3A%20Towards%20a%20mathematical%20model%20of%20tonality.%20Ph.D.%20thesis%2C%20Massachusetts%20Institute%20of%20Technology%2C%20Cambridge%2C%20MA%20%282000%29)

> Chew, E.  
> Modeling Tonality: applications to music cognition.  
> in Moore, J.D., Stenning, K. (eds.) Proceedings of the 23rd Annual Meeting of the Cognitive Science Society, pp. 206–211. Lawrence Erlbaum Assoc. Pub., Edinburgh (2001)
[Google Scholar](https://scholar.google.com/scholar?q=Chew%2C%20E.%3A%20Modeling%20Tonality%3A%20applications%20to%20music%20cognition.%20In%3A%20Moore%2C%20J.D.%2C%20Stenning%2C%20K.%20%28eds.%29%20Proceedings%20of%20the%2023rd%20Annual%20Meeting%20of%20the%20Cognitive%20Science%20Society%2C%20pp.%20206%E2%80%93211.%20Lawrence%20Erlbaum%20Assoc.%20Pub.%2C%20Edinburgh%20%282001%29)

> Chew, E.  
> The Spiral Array: an algorithm for determining key boundaries.  
> in Anagnostopoulou, C., Ferrand, M., Smaill, A. (eds.) Music and Artificial Intelligence—Proceedings of the Second International Conference on Music and Artificial Intelligence, pp. 18–31. Springer, Heidelberg (2002)
[Google Scholar](https://scholar.google.com/scholar?q=Chew%2C%20E.%3A%20The%20Spiral%20Array%3A%20an%20algorithm%20for%20determining%20key%20boundaries.%20In%3A%20Anagnostopoulou%2C%20C.%2C%20Ferrand%2C%20M.%2C%20Smaill%2C%20A.%20%28eds.%29%20Music%20and%20Artificial%20Intelligence%E2%80%94Proceedings%20of%20the%20Second%20International%20Conference%20on%20Music%20and%20Artificial%20Intelligence%2C%20pp.%2018%E2%80%9331.%20Springer%2C%20Heidelberg%20%282002%29)

> Chew, E., Chen, Y.-C.  
> Mapping MIDI to the Spiral Array: disambiguating pitch spellings
> Computational Modeling and Problem Solving in the Networked  World–Proceedings of the 8th INFORMS Computer Society Conference, pp. 259–275
> Kluwer Academic Publishers (2003)
[Google Scholar](https://scholar.google.com/scholar?q=Chew%2C%20E.%2C%20Chen%2C%20Y.-C.%3A%20Mapping%20MIDI%20to%20the%20Spiral%20Array%3A%20disambiguating%20pitch%20spellings.%20In%3A%20Computational%20Modeling%20and%20Problem%20Solving%20in%20the%20Networked%20World%E2%80%93Proceedings%20of%20the%208th%20INFORMS%20Computer%20Society%20Conference%2C%20pp.%20259%E2%80%93275%2C%20Kluwer%20Academic%20Publishers%20%282003%29)

see also
https://en.wikipedia.org/wiki/Spiral_array_model


