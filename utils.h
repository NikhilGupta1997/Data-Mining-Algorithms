/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2011-2016 Jose Luis Blanco (joseluisblancoc@gmail.com).
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

#include <cstdlib>
#include <iostream>
#include <vector>

/* Dim 1 */
template <typename T>
struct PointCloud1
{
	struct Point
	{
		T  x;
	};

	std::vector<Point>  pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, const size_t dim = 0) const
	{
		return pts[idx].x;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void addpoints1(PointCloud1<T> &point, std::vector<std::vector<T>> val)
{
	struct Point
	{
		T  x;
	};
	int N = val.size();
	point.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		point.pts[i].x = val[i][0];
		// std::cout << val[i][0]<<std::endl;
	}
}

/* Dim 2 */
template <typename T>
struct PointCloud2
{
	struct Point
	{
		T  x,y;
	};

	std::vector<Point>  pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim == 0) return pts[idx].x;
		else return pts[idx].y;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void addpoints2(PointCloud2<T> &point, std::vector<std::vector<T>> val)
{
	struct Point
	{
		T  x,y;
	};
	int N = val.size();
	point.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		point.pts[i].x = val[i][0];
		point.pts[i].y = val[i][1];
		// std::cout << val[i][0]<< " " << val[i][1]<<std::endl;
	}
}

/* Dim 3 */
template <typename T>
struct PointCloud3
{
	struct Point
	{
		T  x,y,z;
	};

	std::vector<Point>  pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim == 0) return pts[idx].x;
		else if (dim == 1) return pts[idx].y;
		else return pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void addpoints3(PointCloud3<T> &point, std::vector<std::vector<T>> val)
{
	struct Point
	{
		T  x,y,z;
	};
	int N = val.size();
	point.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		point.pts[i].x = val[i][0];
		point.pts[i].y = val[i][1];
		point.pts[i].z = val[i][2];
		// std::cout << val[i][0]<< " " << val[i][1] << " " << val[i][2]<<std::endl;
	}
}

/* Dim 4 */
template <typename T>
struct PointCloud4
{
	struct Point
	{
		T  w,x,y,z;
	};

	std::vector<Point> pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim==0) return pts[idx].w;
		else if (dim==1) return pts[idx].x;
		else if (dim==2) return pts[idx].y;
		else return pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void addpoints4(PointCloud4<T> &point, std::vector<std::vector<T>> val)
{
	struct Point
	{
		T  w,x,y,z;
	};
	int N = val.size();
	point.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		point.pts[i].w = val[i][0];
		point.pts[i].x = val[i][1];
		point.pts[i].y = val[i][2];
		point.pts[i].z = val[i][3];
		// std::cout << val[i][0]<< " " << val[i][1] << " " << val[i][2] << " " << val[i][3]<<std::endl;
	}
}

/* Dim 5 */
template <typename T>
struct PointCloud5
{
	struct Point
	{
		T  v,w,x,y,z;
	};

	std::vector<Point> pts;

	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim==0) return pts[idx].v;
		else if (dim==1) return pts[idx].w;
		else if (dim==2) return pts[idx].x;
		else if (dim==3) return pts[idx].y;
		else return pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }

};

template <typename T>
void addpoints5(PointCloud5<T> &point, std::vector<std::vector<T>> val)
{
	struct Point
	{
		T  v,w,x,y,z;
	};
	int N = val.size();
	point.pts.resize(N);
	for (size_t i = 0; i < N; i++)
	{
		point.pts[i].v = val[i][0];
		point.pts[i].w = val[i][1];
		point.pts[i].x = val[i][2];
		point.pts[i].y = val[i][3];
		point.pts[i].z = val[i][4];
		// std::cout << val[i][0]<< " " << val[i][1] << " " << val[i][2] << " " << val[i][3] << " " << val[i][4]<<std::endl;
	}
}

void dump_mem_usage()
{
	FILE* f = fopen("/proc/self/statm","rt");
	if (!f) return;
	char str[300];
	size_t n = fread(str, 1, 200, f);
	str[n] = 0;
	printf("MEM: %s\n", str);
	fclose(f);
}
