#pragma once

#include "export.hpp"

#include <chrono>
#include <mutex>
#include <string>

namespace LibIO::Clipboard {
    class LIBIO_API ClipboardControls {
    public:
        /**
         * Houd dit object vast voor de duur van een complete clipboard-transactie:
         * clear -> copy -> ctrl+v, of klik -> lezen. Alles wat de clipboard-inhoud
         * kan wijzigen hoort binnen dezelfde transactie te vallen, inclusief de
         * muisklik die de game zelf naar de clipboard laat schrijven.
         *
         * Copy/Paste/Clear nemen dezelfde recursieve lock, dus nesten mag.
         */
        class LIBIO_API Transaction {
        public:
            Transaction();

            ~Transaction();

            Transaction(const Transaction &) = delete;

            Transaction &operator=(const Transaction &) = delete;
        };

        virtual ~ClipboardControls() = default;

        static std::recursive_mutex &Mutex();

        void Copy(std::string text);

        std::string Paste();

        void Clear();

        /**
         * Copy plus wachten tot de backend onze tekst daadwerkelijk als
         * clipboard-inhoud teruggeeft. Pas daarna is het veilig om ctrl+v naar
         * de game te sturen. False bij timeout.
         */
        bool CopyAndConfirm(const std::string &text,
                            std::chrono::milliseconds timeout = std::chrono::milliseconds(750));

        /**
         * Wachten tot de clipboard-inhoud afwijkt van `previous` en niet leeg is.
         * Voor de situatie waarin de game zelf schrijft na een klik. Lege string
         * bij timeout.
         */
        std::string WaitForChange(const std::string &previous,
                                  std::chrono::milliseconds timeout = std::chrono::milliseconds(500));

    protected:
        virtual void DoCopy(std::string text) = 0;

        virtual std::string DoPaste() = 0;

        virtual void DoClear() = 0;
    };
}
