(ns day-07
  "Day 7: Camel Cards"
  (:require aoc
            [clojure.string :as str]))

(def input (aoc/read-input 7))
(def test-input (aoc/read-input 7 :test))

(defn index-of
  "Return the first index (0-based) at which elem occurs in coll."
  [elem coll]
  (let [len (count coll)
        idx (count (take-while (partial not= elem) coll))]
    (if (= idx len)
      nil ; Elem not found
      idx)))

(defn compare-strength
  [hand-a hand-b]
  (let [card-strength (fn [card]
                        (index-of card
                                  (if (re-find #"" (str card))
                                    [\A \K \Q \J \T]
                                    (range 9 (dec 2) -1))))]
    (mapv (fn [a b]
            (println a b)
            (compare (card-strength a) (card-strength b)))
          hand-a hand-b)))

(def card-values {\A 13 \K 12 \Q 11 \J 10 \T 9
                  \9 8 \8 7 \7 6 \6 5 \5 4 \4 3 \3 2 \2 1})

(defn compare-cards
  [card-a card-b]
  (compare (card-values card-a) (card-values card-b)))

(defn compare-hands
  [hand-a hand-b]
  (loop [cards-a (seq hand-a)
         cards-b (seq hand-b)]
    (cond
      (empty? cards-a) (if (empty? cards-b) 0 -1)
      (empty? cards-b) 1
      :else
      (let [card-comparison (compare-cards (first cards-a) (first cards-b))]
        (if (zero? card-comparison)
          (recur (rest cards-a) (rest cards-b))
          card-comparison)))))

(defn n-to-keyword
  [n rest-of]
  (let [n-words {1 "one", 2 "two", 3 "three", 4 "four", 5 "five"}]
    (keyword (str (get n-words n) "-" rest-of))))

(defn n-of-a-kind?
  ([hand n]
   (n-of-a-kind? hand n true))
  ([hand n unique]
   (let [freqs (frequencies hand)
         n-kind (filter (fn [[elem freq]]
                          (= n freq))
                        freqs)
         unique-other-kind (every? #(<= (second %) 1)
                                   (filter (fn [[elem freq]]
                                             (not= n freq))
                                           freqs))]
     (println n-kind)
     (println unique-other-kind)
     (if unique
       (when (and (->> n-kind (count) (zero?) (not)) ; Check n-kind count is non-zero
                  unique-other-kind)
         (n-to-keyword n "of-a-kind"))
       (when (->> n-kind (count) (zero?) (not))
         (n-to-keyword n "of-a-kind"))
       ))))

(defn full-house?
  [hand]
  (let [freqs (frequencies hand)
        has-three (some #(= 3 %) (vals freqs))
        has-two (some #(= 2 %) (vals freqs))]
    (when (and has-three has-two)
      :full-house)))

(defn n-pair?
  [hand n]
  (let [freqs (frequencies hand)
        pairs (filter #(= 2 %) (vals freqs))]
    (when (= (count pairs) n)
      (n-to-keyword n "pair"))))

(defn high-card?
  [hand]
  (when (distinct? hand)
    :high-card))

(defn hand-type
  [hand]
  (or (n-of-a-kind? hand 5)
      (n-of-a-kind? hand 4)
      (full-house? hand)
      (n-of-a-kind? hand 3)
      (n-pair? hand 2)
      (n-pair? hand 1)
      (high-card? hand)))

;;; I'm calling each line with the hand, bid, & type a camel.
(defn read-camel
  [line]
  (let [[hand bid] (re-seq #"[^\s]+" line)]
    (println line)
    (println "Hand: " hand "| Bid:" bid)
    {:hand hand :hand-type (hand-type hand) :bid (parse-long bid)}))

(defn solve
  [input]
  (sort (fn [a b]
          (println a "\n" b)
          (compare-strength (:hand a) (:hand b)))
        (mapv read-camel input)))

(comment
  (let [camels (flatten
                (mapv (fn [[type hands]]
                        (println "Type:" type "| Hands:" hands)
                        (sort (fn [a b]
                                (day-07/compare-hands b a)) hands))
                      (sort (fn [[type-a] [type-b]]
                              (let [types [:five-of-a-kind :four-of-a-kind
                                           :full-house :three-of-a-kind
                                           :two-pair :one-pair :high-card]]
                                (compare (day-07/index-of type-b types) (day-07/index-of type-a types))))
                            
                            (group-by :hand-type (mapv day-07/read-camel day-07/test-input)))))]
    (reduce (fn [acc [rank camel]]
              (+ acc (* (:bid camel) rank)))
            0        
            (map-indexed #(vector (- (count camels) %1) %2) camels))))

(comment
  ([5 {:hand "T55J5", :hand-type :three-of-a-kind, :bid 684}] ; T55J5 should be 4
   [4 {:hand "QQQJA", :hand-type :three-of-a-kind, :bid 483}] ; QQQJA should be 5
   [3 {:hand "KK677", :hand-type :two-pair, :bid 28}]
   [2 {:hand "KTJJT", :hand-type :two-pair, :bid 220}]
   [1 {:hand "32T3K", :hand-type :one-pair, :bid 765}]))
