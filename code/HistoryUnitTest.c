#include "unit_test_util.h"
#include "History.h"
#include <stdbool.h>

static bool test1() { // creates, inserts and destroys
	History *h = createHistory();
	int a[] = {1,5};
	Move *m = createMove(a,a,'_');
	ASSERT_TRUE(h != NULL);
	ASSERT_TRUE(isEmpty(h));
	insertLast(h, m);
	a[0] = 4;
	m = createMove(a,a,'_');
	insertLast(h, m);
	a[1] = 7;
	m = createMove(a,a,'_');
	insertLast(h, m);
	a[1] = 9;
	m = createMove(a,a,'_');
	insertLast(h, m);
	printHistory(h); // should be:  c: (1,5) -> (1,5) ;d: (4,5) -> (4,5) ;k: (4,7) -> (4,7) ;l: (4,9) -> (4,9) ;
	destroyHistory(h);
	return true;
}

static bool test2a() {
	History *h = createHistory();
	int s[] = {1,5}, d[] = {4,9};
	ASSERT_TRUE(h != NULL);
	ASSERT_TRUE(isEmpty(h));
	insertLastManual(h, s, d,'_');
	removeFirst(h);
	destroyHistory(h);
	return true;
}

static bool test2b() { // creates, inserts manually, deletes (first) and destroys
	History *h = createHistory();
	int i;
	int s[] = {1,5}, d[] = {4,9};
	ASSERT_TRUE(h != NULL);
	ASSERT_TRUE(isEmpty(h));
	for (i = 0 ; i < 6 ; i++) {
		insertLastManual(h, s, d,'_');
		s[0] += i;
		d[1] = i;
	}
	ASSERT_TRUE(isFull(h));
	printHistory(h); // should be:  a:(1,5)->(4,9) ; b:(1,5)->(4,0); c:(2,5)->(4,1); d:(4,5)->(4,2); e:(7,5)->(4,3), f:(11,5)->(4,4) ;
	for (i = 0 ; i < 6 ; i++) {
		removeFirst(h);
	}
	ASSERT_TRUE(isEmpty(h));
	destroyHistory(h);
	return true;
}

static bool test3() { // creates, inserts, deletes (last) and destroys
	History *h = createHistory();
	int i, s[2] = {0,0}, d[2] = {0,0};
	Move *m;
	for (i = 0 ; i < 5 ; i++) {
		m = createMove(s,d,'_');
		insertLast(h,m);
		s[0] = i;
		d[1] = i;
	}
	printHistory(h);
	for (i = 0 ; i < 5 ; i++) {
		removeLast(h);
	}
	ASSERT_TRUE(isEmpty(h));
	destroyHistory(h);
	return true;
}
/*
int main () {
	RUN_TEST(test1);
	RUN_TEST(test2a);
	RUN_TEST(test2b);
	RUN_TEST(test3);
	return 0;
}*/
