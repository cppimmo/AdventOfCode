(ns advent-of-code-2023.day-01
  (:require [clojure.java.io :as io]
            [clojure.string :as str]))

(def ^:dynamic *part* 'one) ; 'one or 'two
(def filename "resources/day_01.txt")

(def word-digits {"one"   1
                  "two"   2
                  "three" 3
                  "four"  4
                  "five"  5
                  "six"   6
                  "seven" 7
                  "eight" 8
                  "nine"  9})

(defn str->int
  [^String s]
  (try
    (Integer/parseInt s)
    (catch NumberFormatException _
      false)))

(defn calibrate
  "Form a two-digit number from the given seq, digits."
  [digits]
  (+ (* 10 (first digits)) (last digits)))

(defn decode
  "Extract calibration value from the string, s."
  [s]
  (if (= *part* 'one)
    (do (let [digits (->> s
                          (str/trim)
                          (re-seq #"\d")
                          (map #(Integer/parseInt %)))]
          (calibrate digits)))
    (do (let [matches (->> s
                           (re-seq
                            #"(?=(one|two|three|four|five|six|seven|eight|nine|\d))")
                           (flatten)
                           (remove empty?))
              digits (map (fn [match]
                            (or (get word-digits match)
                                (str->int match)))
                          matches)]
          (if (empty? digits)
            0
            (calibrate digits))))))

(defn read-input
  [^String f]
  (with-open [rdr (io/reader f)]
    (->> (line-seq rdr)
         (map decode) ; 
         (doall)))) ; Ensure lazy-seq is eval'd

(comment ; Test case:
  (assert (= 281 (reduce + (map decode
                                (list "two1nine"
                                      "eightwothree"
                                      "abcone2threexyz"
                                      "xtwone3four"
                                      "4nineeightseven2"
                                      "zoneight234"
                                      "7pqrstsixteen"))))))

(defn solve
  []
  (reduce + (read-input filename)))

(println "Answer 1:" (solve))
(println "Answer 2:" (binding [*part* 'two] (solve)))

