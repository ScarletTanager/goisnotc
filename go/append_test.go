package main

import (
	"testing"
)

var src []int

func init() {
	src = make([]int, 200000000)
	for i := 0; i < 200000000; i++ {
		src[i] = i
	}
}

func DoAppend(dst, src []int) []int {
	dst = append(dst, src...)
	return dst
}

func DoAppend1x1(dst, src []int) []int {
	for _, val := range src {
		dst = append(dst, val)
	}
	return dst
}

func DoChunkedAppend(dst, src []int, chunksize int) []int {
	upper := int(len(src) / chunksize)
	for i := 0; i < upper; i++ {
		dst = append(dst, src[(chunksize*i):(chunksize*i)+chunksize]...)
	}
	if (upper * chunksize) < len(src) {
		dst = append(dst, src[(upper*chunksize):]...)
	}
	return dst
}

func DoCopyByIndex(dst, src []int) []int {
	for i := 0; i < len(src); i++ {
		dst[i] = src[i]
	}
	return dst
}

func DoCopy(dst, src []int) []int {
	copy(dst, src)
	return dst
}

func makeDestination(sz ...int) []int {
	if len(sz) > 0 {
		return make([]int, sz[0])
	}
	return make([]int, 0)
}

func BenchmarkDoAppendSmall(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:200])
	}
}

func BenchmarkDoAppend1x1Small(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:200])
	}
}

func BenchmarkDoChunkedAppend100Small(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:200], 100)
	}
}

func BenchmarkDoCopyByIndexSmall(b *testing.B) {
	dst := makeDestination(200)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:200])
	}
}

func BenchmarkDoAppendSmallMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:2000])
	}
}

func BenchmarkDoAppend1x1SmallMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:2000])
	}
}

func BenchmarkDoChunkedAppend100SmallMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:2000], 100)
	}
}

func BenchmarkDoCopyByIndexSmallMedium(b *testing.B) {
	dst := makeDestination(2000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:2000])
	}
}

func BenchmarkDoAppendMediumSmall(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:20000])
	}
}

func BenchmarkDoAppend1x1MediumSmall(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:20000])
	}
}

func BenchmarkDoChunkedAppend100MediumSmall(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:20000], 100)
	}
}

func BenchmarkDoCopyByIndexMediumSmall(b *testing.B) {
	dst := makeDestination(20000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:20000])
	}
}

func BenchmarkDoAppendMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:200000])
	}
}

func BenchmarkDoAppend1x1Medium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:200000])
	}
}

func BenchmarkDoChunkedAppend100Medium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:200000], 100)
	}
}

func BenchmarkDoCopyByIndexMedium(b *testing.B) {
	dst := makeDestination(200000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:200000])
	}
}

func BenchmarkDoAppendMediumLarge(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:2000000])
	}
}

func BenchmarkDoAppend1x1MediumLarge(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:2000000])
	}
}

func BenchmarkDoChunkedAppend100MediumLarge(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:2000000], 100)
	}
}

func BenchmarkDoCopyByIndexMediumLarge(b *testing.B) {
	dst := makeDestination(2000000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:2000000])
	}
}

func BenchmarkDoAppendLargeMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:20000000])
	}
}

func BenchmarkDoAppend1x1LargeMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:20000000])
	}
}

func BenchmarkDoChunkedAppend100LargeMedium(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:20000000], 100)
	}
}

func BenchmarkDoCopyByIndexLargeMedium(b *testing.B) {
	dst := makeDestination(20000000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:20000000])
	}
}

func BenchmarkDoAppendLarge(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend(dst, src[0:200000000])
	}
}

func BenchmarkDoAppend1x1Large(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoAppend1x1(dst, src[0:200000000])
	}
}

func BenchmarkDoChunkedAppend100Large(b *testing.B) {
	dst := makeDestination()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoChunkedAppend(dst, src[0:200000000], 100)
	}
}

func BenchmarkDoCopyByIndexLarge(b *testing.B) {
	dst := makeDestination(200000000)
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		DoCopyByIndex(dst, src[0:200000000])
	}
}
