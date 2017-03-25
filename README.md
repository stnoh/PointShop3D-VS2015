# PointShop3D\_bin

This repository extracts the parameterization part of [PointShop3D]. Although it is great tool for point set editing, it is not easy to extract the useful parameterization part.

## License

This work is derivative work of [PointShop3D] and original code is GPL-v2.0 . So, this is also GPL-v2 .

[PointShop3D]:https://graphics.ethz.ch/pointshop3d/

## Usage & specification

It gets 3 (or 4) arguments from a command line.

> PointShop3D_bin.exe {InputMesh.ply} {uv_constraints.txt} {output.txt} {optional.ply}

- InputMesh.ply  
  The mesh we want to parameterize.  
  Right now, it only supports the mesh **with normals & colors**.  
- uv_constraints.txt  
  It sets the *uv*-constraints for parameterization.  
  The format is following:  
  > {vertex id0} {constraint u} {constraint v}  
  > {vertex id1} {constraint u} {constraint v}  
  > ..  
  
- output.txt  
  The text path for getting parameterization result.  
  The format is following:  
  > 0 {u} {v} {b}  
  > 1 {u} {v} {b}  
  > 2 {u} {v} {b}  
  > ...  
  
  *b* denotes the boundary information. If *u* or *v* is exceed [0:1] area, it turns to **1.0**. Otherwise, it keeps **0.0**  
- optional.ply (optional)  
  This is optional argument. It visualize the result parameterization in RGB colored mesh.  
  R-G [0:255] is converted to U-V [0:1] values. Blue channel means the boundary of UV area.  
