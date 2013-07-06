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

def Trapdoor( Key, word, i ):
    return map( lambda x : pesudo_function( str(i) + word , x), Key)

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
        words_count = {}
        for word in self.words:
            if word in words_count.keys():
                words_count[word] = words_count[word]+1
            else:
                words_count[word] = 1
            x = Trapdoor(Kpriv, word, words_count[word])
            y = map( lambda m : pesudo_function(did,m), x)

            for yi in y:
                self.bloomfilter.add(yi)

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

w='This'

t = Trapdoor(Kpriv, w, 2)

print SearchIndex(t,did, bf)

