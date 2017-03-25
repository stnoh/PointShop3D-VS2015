// Title:   Warper_IncludeCode.cpp
// Created: Thu Sep 25 14:49:59 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003 Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

// note: this code is pasted into the diverse WrpWarpBlock_*() functions!


			// note: if A[bp][8] == 0 use non-optimized incremental calculations
			if(A_bp_8 > -NONOPT_TOL && A_bp_8 < NONOPT_TOL)
			{
				while(IITROWS)
				{
					p = p_start_bp;
					q = q_start_bp;
					r = r_start_bp;

					IITFIRSTCOL(r,p,q,dx_r_bp,dx_p_bp,dx_q_bp);
					while(IITCOLS)
					{
						nsamples = IITNSAMPLES;

						// some loop unrolling for the most frequent special case
						if(nsamples == 1)
						{
							// warp sample
							sample = SITGETCURRENT(sit);
							SITNEXT(sit);
							
							sample_depth = SMPGETDEPTH(sample,d);

#ifdef DEMO_CODE
							if(wrp_options & WRP_LODCOLORS)
							{
								// LOD debugging: set a shade index according to block level
								SMPSETWARPEDLEVEL(sample,l);
							}
#endif
#ifdef DEBUG_CDE_CLIP
							if(dbg_outside)
								sample->dbgshadeindex = 1;
							else
								sample->dbgshadeindex = 0;
#endif
	
							// transform normal to camera coordinates
							n_i = SMPGETNORMAL(sample);
							nx_o = wrp_nrm_table[n_i].x;
							ny_o = wrp_nrm_table[n_i].y;
							nz_o = wrp_nrm_table[n_i].z;
	
							n[0] = normalsA[0]*nx_o + normalsA[1]*ny_o + normalsA[2]*nz_o;
							n[1] = normalsA[3]*nx_o + normalsA[4]*ny_o + normalsA[5]*nz_o;
							n[2] = normalsA[6]*nx_o + normalsA[7]*ny_o + normalsA[8]*nz_o;

							// non optimized incremental calculations
							z_c = r + A_bp_8 * sample_depth;
							r_z_c = 1/z_c;
							xImg = (p + A2_vp_sx_bp*sample_depth) * r_z_c + vp_tsx_bp;
							yImg = (q + A5_vp_sy_bp*sample_depth) * r_z_c + vp_tsy_bp;

							// put it into the z-buffer
							// note: no clip testing required
							// caution: this function (or macro) relies on global variables!
							wrp_zbf_splatsurfel(wrp_zbf, xImg, yImg, z_c, n, sample, l,
								stoo_scaling, otoc_scaling, vp_sx, vp_sy, vp_tx[0], vp_ty[0]);
						} else
						{
							while(nsamples)
							{
								// warp sample
								sample = SITGETCURRENT(sit);
								SITNEXT(sit);
								
								sample_depth = SMPGETDEPTH(sample,d);

#ifdef DEMO_CODE
								if(wrp_options & WRP_LODCOLORS)
								{
									// LOD debugging: set a shade index according to block level
									SMPSETWARPEDLEVEL(sample,l);
								}
#endif
#ifdef DEBUG_CDE_CLIP
								if(dbg_outside)
									sample->dbgshadeindex = 1;
								else
									sample->dbgshadeindex = 0;
#endif
							
								// transform normal to camera coordinates
								n_i = SMPGETNORMAL(sample);
								nx_o = wrp_nrm_table[n_i].x;
								ny_o = wrp_nrm_table[n_i].y;
								nz_o = wrp_nrm_table[n_i].z;
	
								n[0] = normalsA[0]*nx_o + normalsA[1]*ny_o + normalsA[2]*nz_o;
								n[1] = normalsA[3]*nx_o + normalsA[4]*ny_o + normalsA[5]*nz_o;
								n[2] = normalsA[6]*nx_o + normalsA[7]*ny_o + normalsA[8]*nz_o;

								// non optimized incremental calculations
								z_c = r + A_bp_8 * sample_depth;
								r_z_c = 1/z_c;
								xImg = (p + A2_vp_sx_bp*sample_depth) * r_z_c + vp_tsx_bp;
								yImg = (q + A5_vp_sy_bp*sample_depth) * r_z_c + vp_tsy_bp;

								// put it into the z-buffer
								// note: no clip testing required
								// caution: this function (or macro) relies on global variables!
								wrp_zbf_splatsurfel(wrp_zbf, xImg, yImg, z_c, n, sample, l,
									stoo_scaling, otoc_scaling, vp_sx, vp_sy, vp_tx[0], vp_ty[0]);

								nsamples--;
							}
						}

						// increment col variables
						IITNEXTCOL(r,p,q,dx_r_bp,dx_p_bp,dx_q_bp);
					}
					// increment row variables
					IITNEXTROW(r_start_bp,p_start_bp,q_start_bp,dy_r_bp,dy_p_bp,dy_q_bp);
				} 
		
			} else
			{
				while(IITROWS)
				{
					p = p_start_bp;
					q = q_start_bp;
					r = r_start_bp;

					IITFIRSTCOL(r,p,q,dx_r_bp,dx_p_bp,dx_q_bp);
					while(IITCOLS)
					{
						
						nsamples = IITNSAMPLES;

						// some loop unrolling for the most frequent special case
						if(nsamples == 1)
						{
							// warp sample
							sample = SITGETCURRENT(sit);
							SITNEXT(sit);
			
							sample_depth = SMPGETDEPTH(sample,d);

#ifdef DEMO_CODE
							if(wrp_options & WRP_LODCOLORS)
							{
								// LOD debugging: set a shade index according to block level
								SMPSETWARPEDLEVEL(sample,l);
							}
#endif
#ifdef DEBUG_CDE_CLIP
							if(dbg_outside)
								sample->dbgshadeindex = 1;
							else
								sample->dbgshadeindex = 0;
#endif

							// transform normal to camera coordinates
							n_i = SMPGETNORMAL(sample);
							nx_o = wrp_nrm_table[n_i].x;
							ny_o = wrp_nrm_table[n_i].y;
							nz_o = wrp_nrm_table[n_i].z;
	
							n[0] = normalsA[0]*nx_o + normalsA[1]*ny_o + normalsA[2]*nz_o;
							n[1] = normalsA[3]*nx_o + normalsA[4]*ny_o + normalsA[5]*nz_o;
							n[2] = normalsA[6]*nx_o + normalsA[7]*ny_o + normalsA[8]*nz_o;

							// use optimized incremental calculations
							z_c = r + A_bp_8 * sample_depth;
							r_z_c = 1/z_c;
							xImg = p * r_z_c + vp_tsx_bp;
							yImg = q * r_z_c + vp_tsy_bp;

							// put it into the z-buffer
							// note: no clip testing required
							// caution: this function (or macro) relies on global variables!
							wrp_zbf_splatsurfel(wrp_zbf, xImg, yImg, z_c, n, sample, l,
								stoo_scaling, otoc_scaling, vp_sx, vp_sy, vp_tx[0], vp_ty[0]);
						} else
						{
							while(nsamples)
							{
								// warp sample
								sample = SITGETCURRENT(sit);
								SITNEXT(sit);
				
								sample_depth = SMPGETDEPTH(sample,d);

#ifdef DEMO_CODE
								if(wrp_options & WRP_LODCOLORS)
								{
									// LOD debugging: set a shade index according to block level
									SMPSETWARPEDLEVEL(sample,l);
								}
#endif
#ifdef DEBUG_CDE_CLIP
								if(dbg_outside)
									sample->dbgshadeindex = 1;
								else
									sample->dbgshadeindex = 0;
#endif

								// transform normal to camera coordinates
								n_i = SMPGETNORMAL(sample);
								nx_o = wrp_nrm_table[n_i].x;
								ny_o = wrp_nrm_table[n_i].y;
								nz_o = wrp_nrm_table[n_i].z;
	
								n[0] = normalsA[0]*nx_o + normalsA[1]*ny_o + normalsA[2]*nz_o;
								n[1] = normalsA[3]*nx_o + normalsA[4]*ny_o + normalsA[5]*nz_o;
								n[2] = normalsA[6]*nx_o + normalsA[7]*ny_o + normalsA[8]*nz_o;

								// use optimized incremental calculations
								z_c = r + A_bp_8 * sample_depth;
								r_z_c = 1/z_c;
								xImg = p * r_z_c + vp_tsx_bp;
								yImg = q * r_z_c + vp_tsy_bp;

								// put it into the z-buffer
								// note: no clip testing required
								// caution: this function (or macro) relies on global variables!
								wrp_zbf_splatsurfel(wrp_zbf, xImg, yImg, z_c, n, sample, l,
									stoo_scaling, otoc_scaling, vp_sx, vp_sy, vp_tx[0], vp_ty[0]);

								nsamples--;
							}
						}

						// increment col variables
						IITNEXTCOL(r,p,q,dx_r_bp,dx_p_bp,dx_q_bp);
					}
					// increment row variables
					IITNEXTROW(r_start_bp,p_start_bp,q_start_bp,dy_r_bp,dy_p_bp,dy_q_bp);
				}
			}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
