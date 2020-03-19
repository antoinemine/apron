# Taylor1plus

Taylor1plus[1] (or t1p) is an **efficient implementation** of the affine forms-based numerical abstract domain introduced by Eric Goubault and Sylvie Putot in [4]. 

It defines the **meet operation** of two affine sets as a *logical product* of standard zonotpes and boxes [2]. 

It also implements several **join operations** over such logical products. 

All computations are enhanced using a (classical) product with the intervals-based abstract domain.    

See [3] for a full account of the underlying theoretical developments (which are only partially implemented in t1p). 

The domain requires the [APRON][apron] library wrappers to work properly. 

You can can also download it as part of [APRON][apron]. 
Both copies are kept sync.  

## References
[1] Ghorbal K, Goubault E, Putot S (2009) The Zonotope Abstract Domain Taylor1+. In CAV 2009, Grenoble, France, June 26 - July 2, 2009. Proceedings. Springer, pp 627–633 
\[[pdf][cav2009]\]

[2] Ghorbal K, Goubault E, Putot S (2010) A Logical Product Approach to Zonotope Intersection. In CAV 2010, Edinburgh, UK, July 15-19, 2010. Proceedings. Springer, pp 212–226 
\[[pdf][cav2010]\]

[3] Ghorbal K (2011) Static Analysis of Numerical Programs: Constrained Affine Sets Abstract Domain. (Analyse Statique de Programmes Numériques: Ensembles Affines Contraints). PhD thesis, École Polytechnique, Palaiseau, France
\[[pdf][thesis]\]

[4] Eric Goubault and Sylvie Putot, Static Analysis of Numerical Algorithms, Proceedings of Static Analysis Symposium SAS'06, Seoul, LNCS volume 4134, pp. 18-34 
\[[pdf][affinedomain]\]

[apron]: http://apron.cri.ensmp.fr/library/
[cav2009]: http://khalilghorbal.info/assets/pdf/papers/cav2009.pdf
[cav2010]: http://khalilghorbal.info/assets/pdf/papers/cav2010.pdf
[thesis]: http://khalilghorbal.info/assets/pdf/papers/thesis-KG.pdf
[affinedomain]: http://www.lix.polytechnique.fr/Labo/Sylvie.Putot/Publications/sas06.pdf
