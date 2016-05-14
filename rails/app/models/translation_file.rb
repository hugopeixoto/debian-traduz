class TranslationFile < ActiveRecord::Base
  belongs_to :repository
  has_many :translations, :dependent => :destroy

  def update_translations
    self.translations.destroy_all
    GetPomo::PoFile.parse(File.read(self.repository.path + "/" + self.filepath))[1..-1].each do |message|
      case message.msgid
        when String
          self.translations.create(msgid: message.msgid, msgstr: message.msgstr)
        when Array
          (0...message.size).each do |i|
            self.translations.create(msgid: message[i].msgid, msgstr: message[i].msgstr)
          end
        else
          raise Exception
      end
    end
  end
end
