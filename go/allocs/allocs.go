package main

import (
	"fmt"
	"sort"
)

var src []int

func init() {
	src = make([]int, 200000000)
	for i := 0; i < 200000000; i++ {
		src[i] = i
	}
}

func main() {
	vals := make(map[int]map[string][]int)
	methods := []string{"1x1", "by100", "by1000", "all"}

	dst := make([]int, 0)
	c := cap(dst)
	for i := 0; i < len(src); i++ {
		dst = append(dst, 1)
		if cap(dst) > c {
			// We've realloc'd
			if _, ok := vals[i+1]; !ok {
				vals[i+1] = make(map[string][]int)
			}
			vals[i+1]["1x1"] = []int{len(dst), cap(dst)}
			c = cap(dst)
		}
	}

	dst = make([]int, 0)
	c = cap(dst)
	for i := 0; i < 2000000; i++ {
		l := 0 + (100 * i)
		u := 100 + (100 * i)
		dst = append(dst, src[l:u]...)
		if cap(dst) > c {
			// We've realloc'd
			if _, ok := vals[u]; !ok {
				vals[u] = make(map[string][]int)
			}
			vals[u]["by100"] = []int{len(dst), cap(dst)}
			c = cap(dst)
		}
	}

	// By 1000s
	dst = make([]int, 0)
	c = cap(dst)
	for i := 0; i < 200000; i++ {
		l := 0 + (1000 * i)
		u := 1000 + (1000 * i)
		dst = append(dst, src[l:u]...)
		if cap(dst) > c {
			// We've realloc'd
			if _, ok := vals[u]; !ok {
				vals[u] = make(map[string][]int)
			}
			vals[u]["by1000"] = []int{len(dst), cap(dst)}
			c = cap(dst)
		}
	}

	dst = make([]int, 0)
	dst = append(dst, src...)
	if _, ok := vals[len(src)]; !ok {
		vals[len(src)] = make(map[string][]int)
	}
	vals[len(src)]["all"] = []int{len(dst), cap(dst)}

	// Build the output
	keys := make([]int, len(vals))
	for k, _ := range vals {
		keys = append(keys, k)
	}
	sort.Ints(keys)
	lines := make([]string, 0, len(keys))
	for _, k := range keys {
		if k < 1 {
			continue
		}
		row := fmt.Sprintf("%10d | ", k)
		for _, m := range methods {
			if v, ok := vals[k][m]; ok {
				row = fmt.Sprintf("%s%10d %10d | ", row, v[0], v[1])
			} else {
				row = fmt.Sprintf("%s    --         --     | ", row)
			}
		}
		lines = append(lines, row)
	}

	fmt.Println("           |          1x1          |          by100       |         by1000        |          all")
	fmt.Println("   index   |     len        cap    |     len       cap    |     len        cap    |     len        cap")
	for _, l := range lines {
		fmt.Println(l)
	}
	fmt.Printf("System pointer size is %d\n", 4<<(^uintptr(0)>>63))
}
