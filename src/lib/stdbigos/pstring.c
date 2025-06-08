#include <stdbigos/pstring.h>
#include <stdbigos/string.h>
#include <stddef.h>

pstring_t pstring_l2w(char* str) {
	return (pstring_t){.len = strlen(str), .data = str};
}

u32 pstring_len_silenterr(const pstring_t* ps) {
	if (!ps || !ps->data) {
		return 0;
	}

	return ps->len;
}

size_t pstring_memcpy(pstring_t* dest, const pstring_t* src) {
	const u32 dest_len = pstring_len_silenterr(dest);
	const u32 src_len = pstring_len_silenterr(src);
	if (dest_len == 0 || src_len == 0) {
		return 0;
	}

	const u32 min_len = (dest_len < src_len) ? dest_len : src_len;
	dest->data = (char*)memcpy(dest->data, src->data, min_len);
	return min_len;
}

u32 pstring_fill(pstring_t* ps, char val) {
	const u32 len = pstring_len_silenterr(ps);
	if (len == 0) {
		return 0;
	}

	for (u32 i = 0; i < len; i++) {
		ps->data[i] = val;
	}

	return len;
}

size_t pstring_memmove(pstring_t* dest, const pstring_t* src, size_t count) {
	const u32 dest_len = pstring_len_silenterr(dest);
	const u32 src_len = pstring_len_silenterr(src);
	if (dest_len == 0 || src_len == 0) {
		return 0;
	}

	const u32 min_len = (src->len < dest->len) ? src->len : dest->len;
	count = (min_len < count) ? min_len : count;

	dest->data = (char*)memmove(dest->data, src->data, count);
	return count;
}

int pstring_cmp(const pstring_t* lhs, const pstring_t* rhs) {
	const u32 lhs_len = pstring_len_silenterr(lhs);
	const u32 rhs_len = pstring_len_silenterr(rhs);
	if (lhs_len != rhs_len) {
		return lhs_len < rhs_len ? -1 : 1;
	}

	size_t l = 0;
	size_t r = 0;
	while (l < lhs_len && r < rhs_len) {
		int cmp = lhs->data[l] - rhs->data[r];
		if (cmp != 0) {
			return cmp;
		}

		l++;
		r++;
	}

	return 0;
}

char* pstring_strchr(const pstring_t* ps, char ch) {
	for (u32 i = 0; i < ps->len; i++) {
		if (ps->data[i] == ch) {
			return ps->data + i;
		}
	}

	return nullptr;
}

pstring_t pstring_slice_view(const pstring_t* ps, u32 start, u32 end) {
	const u32 ps_len = pstring_len_silenterr(ps);
	if (ps_len == 0 || ps_len < start) {
		return (pstring_t){.len = 0, .data = nullptr};
	}

	end = (ps_len < end) ? ps_len : end;
	const u32 new_len = end - start;
	return (pstring_t){.len = new_len, .data = ps->data + start};
}
