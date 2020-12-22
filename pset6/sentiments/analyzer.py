import nltk

class Analyzer():
    """Implements sentiment analysis."""

    #__init__ loads positive and negative words into memory in such a way that analyze can access them
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positive_words=set()
        file=open(positives, "r")
        for line in file:
            if line.startswith(';'):
                continue
            else:
                self.positive_words.add(line.rstrip("\n"))
        file.close()

        self.negative_words=set()
        file=open(negatives, "r")
        for line in file:
            if line.startswith(';'):
                continue
            else:
                self.negative_words.add(line.rstrip("\n"))
        file.close()

    #analyze analyzes the sentiment of text
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score=0
        #nltk
        tokens=nltk.word_tokenize(text)
        for word in tokens:
            #1 if the word is in positives
            if word.lower() in self.positive_words:
                score+=1
            #-1 if the word is in negatives
            elif word.lower() in self.negative_words:
                score-=1
            #0 otherwise
            else:
                continue
        #sum of those values to be the entire text’s score
        return score