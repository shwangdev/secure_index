#!/usr/bin/env python2
import os, sys
import pybloomfilter, hashlib, hmac
r = 8
Kpriv = None
password = '1234'
def pesudo_function(raw, key):
    hashed = hmac.new(key, raw, hashlib.sha1)
    return hashed.hexdigest()

def dimension256(raw):
    sh = hashlib.sha256()
    sh.update(raw)
    return sh.hexdigest()

#Kpriv = map( lambda x: pesudo_function(str(x),password) , range(r))
Kpriv = ['a5d6544aa2e1a658d95d00b5fad454fb28da3599',
         '677809ad2d43752b0a70721105dbaf121e068f9f',
         'a64fb2b43f5e76308a22ce5e374fa676ad35699a',
         'c0e3a021c40aa09f5ebb93f56a56ac418e6aef07']

def Trapdoor( Key, word):
    return map( lambda x : pesudo_function(word , x), Key)

class Document(object):
    def __init__(self, name):
        self.file_name = name
        self.words = [ word  for word in open(self.file_name).read().split()]
        self.uniq_words = list(set (self.words))
        self.bloomfilter = pybloomfilter.BloomFilter(100000, 0.1)
        self.BuildIndex(Kpriv)
        
    def Did(self):
        return "f42a453aea723fb870ff5514311af2ddde4d6f79f91131aa3db2ca39ed59ec72"
        #return dimension256(self.file_name)

    def BuildIndex(self , Key):
        did = self.Did()
        for word in self.uniq_words:
            print word
            x = Trapdoor(Key, dimension256(word))
            y = map( lambda m : pesudo_function(did, m), x)
            for yi in y:
                print yi
                self.bloomfilter.add(yi)
        u = len(self.words)
        v = len(self.uniq_words)
        for i in range( u -v +1 ):
            t = dimension256(str(i))
            self.bloomfilter.add(t)
        return did, self.bloomfilter

    def Index(self):
        return self.Did(), self.bloomfilter

def SearchIndex(t, did, bf):
    y = map(lambda m : pesudo_function(did, m), t)

    for yi in y:
        if yi not in bf:
            return False
    return True
            
d = Document('README.md')
did, bf = d.Index()
print Kpriv
w = 'a'
m = dimension256(w)
print m
t = Trapdoor(Kpriv, m)
print t
print SearchIndex(t,did, bf)

